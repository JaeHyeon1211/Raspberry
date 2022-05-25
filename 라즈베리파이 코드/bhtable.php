<!DOCPYPE html>
<html>
<head>
	<meta charset = "UTF-8">
	<meta http-equiv="refresh" content = "30"> 
    <style type = "text/css">
        .spec{
            text-align:center;
        }
        .con{
            text-align:left;
        }
        </style>
</head>
<body>
    <center><hl align = "center"><b>Big Bear Bath_House</b></hl></center>
    <div class = "spec">
    </div>
    <table border = '1' style = "width = 30%" align = "center">
    <tr align ="center">
        <th>ID</th>
        <th>DATE</th>
        <th>TIME</th>
        <th>TEMPERATURE</th>
        <th>MOISTURE</th>
        <th>WATER LEVEL</th>
        <th>WATER TEMP</th>
    </tr>
	<?php
		$conn = mysqli_connect("localhost","root","kcci");
		mysqli_select_db($conn , "arduinod2");
		$result = mysqli_query($conn, "select * from bath_house ORDER BY id DESC limit 30");

		while($row = mysqli_fetch_array($result)){
        echo "<tr align = center>";
        echo '<td>'.$row['ID'].'</td>';
        echo '<td>'.$row['DATE'].'</td>';
        echo '<td>'.$row['TIME'].'</td>';
        echo '<td>'.$row['TEMP'].'</td>';
        echo '<td>'.$row['MOISTURE'].'</td>';
        echo '<td>'.$row['WATER_LEVEL'].'</td>';
        echo '<td>'.$row['WATER_TEMP'].'</td>';
        echo "</tr>";
        mysqli_close($conn);
		}
		?>
		</teble></body></html>
<button type="button" id="Home" onclick="location.href='index.html'">Home</button>
