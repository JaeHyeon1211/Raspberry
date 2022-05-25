<?php
$conn = mysqli_connect("10.10.141.72:81","root","kcci", "project");
mysqli_set_charset($conn,"utf8");
$sql = "select * from bath_house";
$result=mysqli_query($conn, $sql);
?>
<script src="https://www.gstatic.com/charts/loader.js"></script>
<script type="text/javascript">
google.charts.load('current', {packages: ['corechart']});
google.charts.setOnLoadCallback(drawLineColors);


function drawLineColors() {
var data = new google.visualization.DataTable();
data.addColumn('timeofday', '시간');
data.addColumn('number', '1번');
data.addColumn('number', '2번');
data.addColumn('number', '3번');
data.addColumn('number', '4번');
data.addColumn('number', '5번');
data.addColumn('number', '6번');

data.addRows(
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
);
var options = {
hAxis: {
title: 'Time'
},
vAxis: {
title: 'value'
},
colors: ['#a52714', '#097138', '#097138', '#097138', '#097138', '#097138']
};
var chart = new google.visualization.LineChart(document.getElementById('positive_chart_div'));
chart.draw(data, options);
}
</script>

<div id="positive_chart_div">

</div>