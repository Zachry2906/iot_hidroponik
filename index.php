<?php
include 'config.php';

// Fetch latest 20 records
$sql = "SELECT * FROM sensor_data ORDER BY timestamp DESC LIMIT 20";
$result = $conn->query($sql);
?>

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Water Monitoring Dashboard</title>
    <style>
        body { font-family: Arial, sans-serif; }
        table { width: 100%; border-collapse: collapse; }
        th, td { border: 1px solid #ddd; padding: 8px; text-align: left; }
        th { background-color: #f2f2f2; }
    </style>
</head>
<body>
    <h1>Water Monitoring Dashboard</h1>
    
    <h2>Latest Sensor Readings</h2>
    <table>
        <thead>
            <tr>
                <th>Timestamp</th>
                <th>Water Level</th>
                <th>Distance (cm)</th>
            </tr>
        </thead>
        <tbody>
            <?php while($row = $result->fetch_assoc()): ?>
            <tr>
                <td><?php echo $row['timestamp']; ?></td>
                <td><?php echo $row['water_level']; ?></td>
                <td><?php echo $row['distance']; ?></td>
            </tr>
            <?php endwhile; ?>
        </tbody>
    </table>
</body>
</html>