<?php
/**
 * Smart Farm API - Data Endpoint
 * 
 * Mengembalikan data sensor dalam format JSON untuk dashboard.
 * 
 * Query params:
 *   ?type=latest       → Data terbaru (1 record)
 *   ?type=history&n=50 → Data historis (n record terakhir)
 *   ?type=stats         → Statistik (min, max, avg) hari ini
 */

header('Content-Type: application/json');
header('Access-Control-Allow-Origin: *');

require_once __DIR__ . '/../vendor/autoload.php';
require_once __DIR__ . '/../src/Database.php';

$dbConfig = require __DIR__ . '/../config/database.php';

try {
    $dsn = sprintf(
        'mysql:host=%s;port=%d;dbname=%s;charset=%s',
        $dbConfig['host'],
        $dbConfig['port'],
        $dbConfig['database'],
        $dbConfig['charset']
    );

    $pdo = new PDO($dsn, $dbConfig['username'], $dbConfig['password'], [
        PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
        PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
        PDO::ATTR_EMULATE_PREPARES   => false,
    ]);
} catch (PDOException $e) {
    http_response_code(500);
    echo json_encode(['error' => 'Database connection failed']);
    exit;
}

$type = $_GET['type'] ?? 'latest';

switch ($type) {
    case 'latest':
        $stmt = $pdo->query("SELECT * FROM sensor_data ORDER BY created_at DESC LIMIT 1");
        $data = $stmt->fetch();
        
        // Also get total count and uptime
        $countStmt = $pdo->query("SELECT COUNT(*) as total FROM sensor_data");
        $count = $countStmt->fetch();
        
        $firstStmt = $pdo->query("SELECT created_at FROM sensor_data ORDER BY created_at ASC LIMIT 1");
        $first = $firstStmt->fetch();
        
        echo json_encode([
            'success' => true,
            'data'    => $data ?: null,
            'total_records' => (int)($count['total'] ?? 0),
            'first_record'  => $first['created_at'] ?? null,
            'timestamp' => date('Y-m-d H:i:s'),
        ]);
        break;

    case 'history':
        $limit = min((int)($_GET['n'] ?? 60), 500);
        $stmt = $pdo->prepare("SELECT * FROM sensor_data ORDER BY created_at DESC LIMIT :limit");
        $stmt->bindValue(':limit', $limit, PDO::PARAM_INT);
        $stmt->execute();
        $rows = $stmt->fetchAll();
        
        echo json_encode([
            'success' => true,
            'data'    => array_reverse($rows), // oldest first for charts
            'count'   => count($rows),
        ]);
        break;

    case 'stats':
        $stmt = $pdo->query("
            SELECT 
                MIN(mois_avg) as mois_min, MAX(mois_avg) as mois_max, ROUND(AVG(mois_avg),1) as mois_mean,
                MIN(humidity) as hum_min, MAX(humidity) as hum_max, ROUND(AVG(humidity),1) as hum_mean,
                MIN(temperature) as temp_min, MAX(temperature) as temp_max, ROUND(AVG(temperature),1) as temp_mean,
                MIN(ph_value) as ph_min, MAX(ph_value) as ph_max, ROUND(AVG(ph_value),2) as ph_mean,
                COUNT(*) as total
            FROM sensor_data
            WHERE created_at >= CURDATE()
        ");
        $data = $stmt->fetch();
        
        echo json_encode([
            'success' => true,
            'data'    => $data,
        ]);
        break;

    case 'chart':
        $range = $_GET['range'] ?? 'day';
        
        switch ($range) {
            case 'day':
                // Last 24 hours — raw data, max 500
                $stmt = $pdo->prepare("
                    SELECT id, mois1, mois2, mois3, mois4, mois_avg, humidity, temperature, ph_raw, ph_value, created_at
                    FROM sensor_data 
                    WHERE created_at >= DATE_SUB(NOW(), INTERVAL 1 DAY)
                    ORDER BY created_at ASC
                    LIMIT 500
                ");
                $stmt->execute();
                break;

            case 'week':
                // Last 7 days — aggregate per hour
                $stmt = $pdo->prepare("
                    SELECT 
                        NULL as id,
                        ROUND(AVG(mois1)) as mois1, ROUND(AVG(mois2)) as mois2,
                        ROUND(AVG(mois3)) as mois3, ROUND(AVG(mois4)) as mois4,
                        ROUND(AVG(mois_avg),1) as mois_avg,
                        ROUND(AVG(humidity),1) as humidity,
                        ROUND(AVG(temperature),1) as temperature,
                        ROUND(AVG(ph_raw),1) as ph_raw,
                        ROUND(AVG(ph_value),2) as ph_value,
                        DATE_FORMAT(created_at, '%Y-%m-%d %H:00:00') as created_at
                    FROM sensor_data
                    WHERE created_at >= DATE_SUB(NOW(), INTERVAL 7 DAY)
                    GROUP BY DATE_FORMAT(created_at, '%Y-%m-%d %H')
                    ORDER BY created_at ASC
                ");
                $stmt->execute();
                break;

            case 'month':
                // Last 30 days — aggregate per 6 hours
                $stmt = $pdo->prepare("
                    SELECT 
                        NULL as id,
                        ROUND(AVG(mois1)) as mois1, ROUND(AVG(mois2)) as mois2,
                        ROUND(AVG(mois3)) as mois3, ROUND(AVG(mois4)) as mois4,
                        ROUND(AVG(mois_avg),1) as mois_avg,
                        ROUND(AVG(humidity),1) as humidity,
                        ROUND(AVG(temperature),1) as temperature,
                        ROUND(AVG(ph_raw),1) as ph_raw,
                        ROUND(AVG(ph_value),2) as ph_value,
                        DATE_FORMAT(created_at, '%Y-%m-%d %H:00:00') as created_at
                    FROM sensor_data
                    WHERE created_at >= DATE_SUB(NOW(), INTERVAL 30 DAY)
                    GROUP BY DATE(created_at), FLOOR(HOUR(created_at) / 6)
                    ORDER BY created_at ASC
                ");
                $stmt->execute();
                break;

            case 'all':
            default:
                // All data — aggregate per day
                $stmt = $pdo->prepare("
                    SELECT 
                        NULL as id,
                        ROUND(AVG(mois1)) as mois1, ROUND(AVG(mois2)) as mois2,
                        ROUND(AVG(mois3)) as mois3, ROUND(AVG(mois4)) as mois4,
                        ROUND(AVG(mois_avg),1) as mois_avg,
                        ROUND(AVG(humidity),1) as humidity,
                        ROUND(AVG(temperature),1) as temperature,
                        ROUND(AVG(ph_raw),1) as ph_raw,
                        ROUND(AVG(ph_value),2) as ph_value,
                        DATE_FORMAT(created_at, '%Y-%m-%d') as created_at
                    FROM sensor_data
                    GROUP BY DATE(created_at)
                    ORDER BY created_at ASC
                ");
                $stmt->execute();
                break;
        }

        $rows = $stmt->fetchAll();
        echo json_encode([
            'success' => true,
            'data'    => $rows,
            'count'   => count($rows),
            'range'   => $range,
        ]);
        break;

    case 'download':
        // Return raw data for download (JSON, client converts to Excel)
        $range = $_GET['range'] ?? 'day';
        
        switch ($range) {
            case 'day':
                $where = "WHERE created_at >= DATE_SUB(NOW(), INTERVAL 1 DAY)";
                break;
            case 'week':
                $where = "WHERE created_at >= DATE_SUB(NOW(), INTERVAL 7 DAY)";
                break;
            case 'month':
                $where = "WHERE created_at >= DATE_SUB(NOW(), INTERVAL 30 DAY)";
                break;
            case 'all':
            default:
                $where = "";
                break;
        }

        $stmt = $pdo->query("
            SELECT id, mois1, mois2, mois3, mois4, mois_avg, humidity, temperature, ph_raw, ph_value, created_at
            FROM sensor_data 
            {$where}
            ORDER BY created_at DESC
        ");
        $rows = $stmt->fetchAll();

        echo json_encode([
            'success' => true,
            'data'    => $rows,
            'count'   => count($rows),
            'range'   => $range,
        ]);
        break;

    default:
        http_response_code(400);
        echo json_encode(['error' => 'Invalid type parameter']);
        break;
}
