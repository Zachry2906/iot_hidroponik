<?php
include 'config.php';

if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    $water_level = $_POST['water_level'];
    $distance = $_POST['distance'];

    $sql = "INSERT INTO sensor_data (water_level, distance) VALUES (?, ?)";
    $stmt = $conn->prepare($sql);
    $stmt->bind_param("id", $water_level, $distance);
    
    if ($stmt->execute()) {
        echo "Data inserted successfully";
    } else {
        echo "Error: " . $stmt->error;
    }

    $stmt->close();
    $conn->close();
} else {
    echo "Invalid request method";
}
?>