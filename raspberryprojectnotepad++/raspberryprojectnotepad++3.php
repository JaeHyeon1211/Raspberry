<?php
$conn = mysqli_connect("10.10.141.72:81","root","kcci", "project");
mysqli_set_charset($conn,"utf8");
$sql = "select * from bath_house";
$result=mysqli_query($conn, $sql);
?>
<html>
  <head>
   <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
   <script type="text/javascript">
      google.charts.load('current', {'packages':['gauge']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {

        var data = google.visualization.arrayToDataTable([
		<?php
		$output = '[';
			while($row = mysqli_fetch_array($result)){
			$day=$row["date"];
			$hr= substr($day,11,2);
			$min= substr($day,14,2);
			$sec= substr($day,17,2);
			if ($output !== '[') { $output .= ',' ; }
			$output .= '[['.$hr.','.$min.','.$sec.'],'.$row["1번"].','.$row["2번"].','.$row["3번"].','.$row["4번"].','.$row["5번"].','.$row["6번"].']';
			}
		$output .= ']';
		echo $output;
		$a=json_encode($array);
		mysqli_close($conn);

?>
          ['Label', 'Value'],
          ['Memory', 80],
          ['CPU', 55],
          ['Network', 68]
        ]);

        var options = {
          width: 400, height: 120,
          redFrom: 90, redTo: 100,
          yellowFrom:75, yellowTo: 90,
          minorTicks: 5
        };

        var chart = new google.visualization.Gauge(document.getElementById('chart_div'));

        chart.draw(data, options);

        setInterval(function() {
          data.setValue(0, 1, 40 + Math.round(60 * Math.random()));
          chart.draw(data, options);
        }, 13000);
        setInterval(function() {
          data.setValue(1, 1, 40 + Math.round(60 * Math.random()));
          chart.draw(data, options);
        }, 5000);
        setInterval(function() {
          data.setValue(2, 1, 60 + Math.round(20 * Math.random()));
          chart.draw(data, options);
        }, 26000);
      }
    </script>
  </head>
  <body>
    <div id="chart_div" style="width: 400px; height: 120px;"></div>
  </body>
</html>
