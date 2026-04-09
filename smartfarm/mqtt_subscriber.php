<?php
/**
 * ============================================
 * Smart Farm MQTT Subscriber
 * ============================================
 * 
 * Script ini:
 *   1. Subscribe ke topic MQTT di broker EMQX
 *   2. Menerima data JSON dari ESP32
 *   3. Menyimpan data ke database MySQL
 * 
 * Cara menjalankan:
 *   cd c:\xampp\htdocs\esp32_smartFarm--mainSensor\smartfarm
 *   php mqtt_subscriber.php
 * 
 * Script ini berjalan terus-menerus (long-running process).
 * Tekan Ctrl+C untuk menghentikan.
 */

require_once __DIR__ . '/vendor/autoload.php';
require_once __DIR__ . '/src/Database.php';

use PhpMqtt\Client\MqttClient;
use PhpMqtt\Client\ConnectionSettings;
use SmartFarm\Database;

// ============================================
// Load Konfigurasi
// ============================================
$dbConfig   = require __DIR__ . '/config/database.php';
$mqttConfig = require __DIR__ . '/config/mqtt.php';

// ============================================
// Tampilkan Banner
// ============================================
echo "\n";
echo "╔══════════════════════════════════════════╗\n";
echo "║     🌱 Smart Farm MQTT Subscriber 🌱     ║\n";
echo "╠══════════════════════════════════════════╣\n";
echo "║  Broker : {$mqttConfig['host']}          ║\n";
echo "║  Port   : {$mqttConfig['port']}                      ║\n";
echo "║  Topic  : {$mqttConfig['topic']}  ║\n";                     
echo "╚══════════════════════════════════════════╝\n";
echo "\n";

// ============================================
// Koneksi Database
// ============================================
echo "[INFO] Menghubungkan ke database MySQL...\n";

try {
    $db = new Database($dbConfig);
    echo "[OK]   Database terhubung!\n";
} catch (\RuntimeException $e) {
    echo "[FAIL] " . $e->getMessage() . "\n";
    echo "[HINT] Pastikan:\n";
    echo "       1. XAMPP MySQL sudah berjalan\n";
    echo "       2. Database 'smartfarm_db' sudah dibuat (jalankan sql/create_tables.sql)\n";
    exit(1);
}

// ============================================
// Mapping Key JSON dari ESP32 ke kolom database
// ============================================
// ESP32 mengirim key dengan format "Mois1:", "Hum:", dll.
// Kita mapping ke nama kolom database
$keyMapping = [
    'Mois1:'   => 'mois1',
    'Mois2:'   => 'mois2',
    'Mois3:'   => 'mois3',
    'Mois4:'   => 'mois4',
    'Mois_Av:' => 'mois_av',
    'Hum:'     => 'humidity',
    'Temp:'    => 'temperature',
    'pH:'      => 'ph',
];

// ============================================
// Fungsi: Parse data JSON dari ESP32
// ============================================
function parseEsp32Data(string $jsonPayload, array $keyMapping): ?array
{
    $raw = json_decode($jsonPayload, true);

    if ($raw === null) {
        echo "[WARN] JSON tidak valid: {$jsonPayload}\n";
        return null;
    }

    $mapped = [];
    foreach ($keyMapping as $espKey => $dbColumn) {
        $mapped[$dbColumn] = isset($raw[$espKey]) ? (int)$raw[$espKey] : 0;
    }

    return $mapped;
}

// ============================================
// Koneksi MQTT & Subscribe
// ============================================
echo "[INFO] Menghubungkan ke MQTT broker ({$mqttConfig['host']}:{$mqttConfig['port']})...\n";

try {
    $mqtt = new MqttClient(
        $mqttConfig['host'],
        $mqttConfig['port'],
        $mqttConfig['client_id']
    );

    $connectionSettings = (new ConnectionSettings)
        ->setKeepAliveInterval(60)
        ->setConnectTimeout(10)
        ->setLastWillTopic('smartfarm/php-subscriber/status')
        ->setLastWillMessage('offline')
        ->setLastWillQualityOfService(0);

    // Tambahkan username/password jika ada
    if (!empty($mqttConfig['username'])) {
        $connectionSettings
            ->setUsername($mqttConfig['username'])
            ->setPassword($mqttConfig['password']);
    }

    $mqtt->connect($connectionSettings, true);
    echo "[OK]   MQTT terhubung ke {$mqttConfig['host']}!\n";

} catch (\Exception $e) {
    echo "[FAIL] MQTT koneksi gagal: " . $e->getMessage() . "\n";
    echo "[HINT] Pastikan:\n";
    echo "       1. Kamu terhubung ke internet\n";
    echo "       2. broker.emqx.io bisa diakses\n";
    echo "       3. Port 1883 tidak diblokir firewall\n";
    exit(1);
}

// ============================================
// Subscribe ke Topic
// ============================================
$topic = $mqttConfig['topic'];
$dataCount = 0;

echo "[INFO] Subscribing ke topic: {$topic}\n";
echo "[INFO] Menunggu data dari ESP32...\n";
echo str_repeat("─", 50) . "\n";

// NOTE: Kita tidak bisa pakai callback di subscribe() karena library
// php-mqtt/client menggunakan regex untuk topic matching, dan karakter
// [ ] di topic "main-sensor[FNC]" dianggap sebagai regex character class.
// Solusi: subscribe tanpa callback, dan gunakan event handler sebagai gantinya.

$mqtt->subscribe($topic, null, 0);

// Event handler ini akan dipanggil untuk SETIAP pesan yang diterima
$mqtt->registerMessageReceivedEventHandler(
    function (MqttClient $client, string $topic, string $message) use ($db, $keyMapping, &$dataCount, $mqttConfig) {
        // Filter hanya topic yang kita inginkan
        if ($topic !== $mqttConfig['topic']) {
            return;
        }

        $dataCount++;
        $timestamp = date('Y-m-d H:i:s');

        echo "\n[DATA #{$dataCount}] {$timestamp}\n";
        echo "  Topic   : {$topic}\n";
        echo "  Payload : {$message}\n";

        // Parse JSON dari ESP32
        $sensorData = parseEsp32Data($message, $keyMapping);

        if ($sensorData === null) {
            echo "  Status  : ❌ Data tidak valid, dilewati\n";
            return;
        }

        // Tampilkan data yang sudah di-parse
        echo "  ┌─────────────────────────────────┐\n";
        echo sprintf("  │ Moisture 1  : %-3d%%              │\n", $sensorData['mois1']);
        echo sprintf("  │ Moisture 2  : %-3d%%              │\n", $sensorData['mois2']);
        echo sprintf("  │ Moisture 3  : %-3d%%              │\n", $sensorData['mois3']);
        echo sprintf("  │ Moisture 4  : %-3d%%              │\n", $sensorData['mois4']);
        echo sprintf("  │ Moisture Avg: %-3d%%              │\n", $sensorData['mois_av']);
        echo sprintf("  │ Humidity    : %-3d%%              │\n", $sensorData['humidity']);
        echo sprintf("  │ Temperature : %-3d°C             │\n", $sensorData['temperature']);
        echo sprintf("  │ pH Level    : %-3d               │\n", $sensorData['ph']);
        echo "  └─────────────────────────────────┘\n";

        // Simpan ke Database
        if ($db->insertSensorData($sensorData)) {
            echo "  Status  : ✅ Tersimpan ke database\n";
        } else {
            echo "  Status  : ❌ Gagal menyimpan ke database\n";
        }
    }
);

// ============================================
// Loop Utama - Menunggu pesan MQTT
// ============================================
echo "\n[INFO] 🟢 Subscriber aktif! Menunggu data dari ESP32...\n";
echo "[INFO] Tekan Ctrl+C untuk menghentikan.\n\n";

// Jalankan loop MQTT (blocking)
// Akan terus berjalan sampai Ctrl+C
try {
    $mqtt->loop(true);
} catch (\Exception $e) {
    echo "\n[INFO] Subscriber dihentikan: " . $e->getMessage() . "\n";
} finally {
    $mqtt->disconnect();
    echo "[INFO] MQTT disconnected. Selesai.\n";
}
