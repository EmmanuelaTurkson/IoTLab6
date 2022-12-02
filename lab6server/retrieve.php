<?php
require "db.php";


    if (!isset($_GET['LocationID'])) 
        die(" Location not specified");
    if ($_GET['LocationID']=='')
        die(" Location is blank");
    if (!is_numeric($_GET['LocationID'] ) )
        die(" Location is not numeric");
    
    $dbname="IoTsix";
    $con = mysqli_connect($servername,$username,$password,$dbname);

    
    $data=array();
    $q=mysqli_query($con,"select * from readings where location_id={$_GET['LocationID']} ORDER BY time DESC LIMIT 10");

    //echo "table"

    $row=mysqli_fetch_object($q);
    echo "<table>
              <tr> 
                      <th>Location ID</th> 
                      <th>Temperature</th>
                      <th>Depth</th>
                      <th>Time</th>
              </tr>";

        while ($row){
        echo"<tr style>
                        <td>{$row->location_id}</td> 
                        <td>{$row->temperature}</td>
                        <td>{$row->distance} </td> 
                        <td>{$row->time} </td> 
                </tr>";
        $row=mysqli_fetch_object($q);
        }
    echo "</table>";      
  
?>