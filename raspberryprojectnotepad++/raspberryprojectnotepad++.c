 char *pArray[4] = {0};
 char *pToken= 0;
    printf("raspberry Start");
    fflush(stdout);
    if((fd = serialOpen(device,baud)) < 0){
        fprintf(stderr, "Unable %s\n",strerror(errno));
        exit(1);
    }
    if(wiringPiSetup() == -1)
        return 1;
    while(1)
    {
        if(serialDataAvail(fd))
        {
            ser_buff[index++] = serialGetchar(fd);
            if(ser_buff[index-1] == 'L')
            {
                ser_buff[index-1] = '\0';
                str_len = strlen(ser_buff);
                printf("ser_buff = %s\n", ser_buff);
                pToken = strtok(ser_buff, ":");
                int i = 0;
                while(pToken != NULL)
                {
                    pArray[i] = pToken;
                    if(++i>4)
                    {
                        break;
                    }
                    pToken = strtok(NULL, ":");
                }
                temp = atoi(pArray[0]);
                humi = atoi(pArray[1]);
                water = atoi(pArray[2]);
                temp_W = atoi(pArray[3]);
                printf("temp = %d, humi = %d, water = %d, temp_W = %d\n",temp,humi,water,temp_W);
                for(i = 0; i<=str_len; i++)
                {
                    ser_buff[i] = 0;
                    index = 0;
                }
                fflush(stdout);
            }
        }
    }
    return 0;
}