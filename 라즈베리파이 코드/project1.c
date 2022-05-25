#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include <mysql/mysql.h>

static char* host = "localhost";
static char* user = "root";
static char* pass = "kcci";
static char* dbname = "arduinod2";

char device[] = "/dev/ttyUSB0";
int fd;
unsigned long baud = 9600;

int main() {
    MYSQL* conn;
    conn = mysql_init(NULL);
    int sql_index, flag = 0;
    char in_sql[200] = { 0 };
    int res = 0;

    if (!(mysql_real_connect(conn, host, user, pass, dbname, 0, NULL, 0)))
    {
        fprintf(stderr, "error : %s[%d]\n", mysql_error(conn), mysql_errno(conn));
        exit(1);
    }
    else
        printf("connection  success");

    char ser_buff[20] = { 0 };
    long int  index = 0, date, time, temp, humi, water_Level, water_Temp, str_len;
    char* pArray[6] = { 0 };
    char* pToken = 0;
    printf("raspberry Start");
    fflush(stdout);
    if ((fd = serialOpen(device, baud)) < 0) {
        fprintf(stderr, "Unable %s\n", strerror(errno));
        exit(1);
    }
    if (wiringPiSetup() == -1)
        return 1;
    while (1)
    {
        if (serialDataAvail(fd))
        {
            ser_buff[index++] = serialGetchar(fd);
            if (ser_buff[index - 1] == 'L')
            {
                flag = 1;
                ser_buff[index - 1] = '\0';
                str_len = strlen(ser_buff);
                pToken = strtok(ser_buff, ":");
                int i = 0;
                while (pToken != NULL)
                {
                    pArray[i] = pToken;
                    if (++i > 6)
                    {
                        break;
                    }
                    pToken = strtok(NULL, ":");
                }
                date = atoi(pArray[0]);
                time = atoi(pArray[1]);
                temp = atoi(pArray[2]);
                humi = atoi(pArray[3]);
                water_Level = atoi(pArray[4]);
                water_Temp = atoi(pArray[5]);
                printf("date = %d, time = %d, temp = %d, humi = %d, water = %d, temp_W = %d\n", date, time, temp, humi, water_Level, water_Temp);
                for (i = 0; i <= str_len; i++)
                {
                    ser_buff[i] = 0;
                    index = 0;
                }
            }
            if (temp < 100 && humi < 100 && water_Level < 800 && water_Temp < 65)
            {
                if (flag == 1)
                {
                    sprintf(in_sql, "insert into bath_house (ID, DATE, TIME,TEMP, MOISTURE, WATER_LEVEL, WATER_TEMP) values(null, % d, % d, % d, % d, % d, % d)",date, time, temp,humi, water_Level, water_Temp);
                        res = mysql_query(conn, in_sql);
                    if (!res)
                    {
                    }
                    else
                    {
                        fprintf(stderr, "error: %s[%d]\n", mysql_error(conn), mysql_errno(conn));
                        exit(1);
                    }
                }
            }
        }
        flag = 0;
        fflush(stdout);
    }
    mysql_close(conn);
    return EXIT_SUCCESS;
}