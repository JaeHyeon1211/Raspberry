<?php
$conn = mysqli_connect("localhost","root","kcci");
mysqli_set_charset($conn,"utf8");
mysqli_select_db($conn,"arduinod2");
$result=mysqli_query($conn, "select DATE, TIME, TEMP, MOISTURE from bath_house");
$data = array(array('arduinod2','온도','습도'));
if($result){
    while($row=mysqli_fetch_array($result))
    {
        array_push($data, array($row['DATE']."\n".$row[1],intval($row[2]),intval($row[3])));
    }
}
$options = array(
    'title' => '온도 (단위:섭씨)','width' => 1000, 'height' => 500
);
?>
<script src="//www.google.com/jsapi"></script>
<script>
    let data = <?= json_encode($data) ?>;
    let options = <?= json_encode($options) ?>;
    google.load('visualization', '1.0', {'packages':['corechart']});
    google.setOnLoadCallback(function() {
        let chart = new google.visualization.LineChart(document.querySelector('#chart_div'));
        chart.draw(google.visualization.arrayToDataTable(data), options);
    });
</script>
<div id="chart_div"></div>
<button type="button" id="Home" onclick="location.href='index.html'">Home</button>
