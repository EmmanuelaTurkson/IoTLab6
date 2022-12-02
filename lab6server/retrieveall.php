<?php
require "db.php";



    
    $dbname="IoTsix";
    $con = mysqli_connect($servername,$username,$password,$dbname);

    
    $data=array();
    $q=mysqli_query($con,"select * from readings ORDER BY time DESC");

    //echo "table"

    $row=mysqli_fetch_object($q);
    echo "<table>
              <tr> 
                      <th>tank number</th> 
                      <th>location</th>
                      <th>depth</th>
                      <th>time</th>
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