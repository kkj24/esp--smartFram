<?php
/**
 * Database Class
 * 
 * Mengelola koneksi PDO ke MySQL dan operasi insert data sensor.
 */

namespace SmartFarm;

use PDO;
use PDOException;

class Database
{
    private PDO $pdo;

    /**
     * Buat koneksi database dari config
     */
    public function __construct(array $config)
    {
        $dsn = sprintf(
            'mysql:host=%s;port=%d;dbname=%s;charset=%s',
            $config['host'],
            $config['port'],
            $config['database'],
            $config['charset']
        );

        try {
            $this->pdo = new PDO($dsn, $config['username'], $config['password'], [
                PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
                PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
                PDO::ATTR_EMULATE_PREPARES   => false,
            ]);
        } catch (PDOException $e) {
            throw new \RuntimeException("[DB] Koneksi gagal: " . $e->getMessage());
        }
    }

    /**
     * Insert data sensor ke tabel sensor_data
     *
     * @param array $data Associative array dengan keys:
     *   mois1, mois2, mois3, mois4, mois_av, humidity, temperature, ph
     * @return bool
     */
    public function insertSensorData(array $data): bool
    {
        $sql = "INSERT INTO sensor_data 
                    (mois1, mois2, mois3, mois4, mois_avg, humidity, temperature, ph_raw)
                VALUES 
                    (:mois1, :mois2, :mois3, :mois4, :mois_avg, :humidity, :temperature, :ph_raw)";

        try {
            $stmt = $this->pdo->prepare($sql);
            return $stmt->execute([
                ':mois1'       => $data['mois1']       ?? 0,
                ':mois2'       => $data['mois2']       ?? 0,
                ':mois3'       => $data['mois3']       ?? 0,
                ':mois4'       => $data['mois4']       ?? 0,
                ':mois_avg'    => $data['mois_av']     ?? 0,
                ':humidity'    => $data['humidity']     ?? 0,
                ':temperature' => $data['temperature'] ?? 0,
                ':ph_raw'      => $data['ph']          ?? 0,
            ]);
        } catch (PDOException $e) {
            echo "[DB] Insert gagal: " . $e->getMessage() . PHP_EOL;
            return false;
        }
    }

    /**
     * Ambil data sensor terbaru
     *
     * @param int $limit Jumlah data yang diambil
     * @return array
     */
    public function getLatestData(int $limit = 10): array
    {
        $sql = "SELECT * FROM sensor_data ORDER BY created_at DESC LIMIT :limit";
        $stmt = $this->pdo->prepare($sql);
        $stmt->bindValue(':limit', $limit, PDO::PARAM_INT);
        $stmt->execute();
        return $stmt->fetchAll();
    }

    /**
     * Cek koneksi database
     */
    public function isConnected(): bool
    {
        try {
            $this->pdo->query('SELECT 1');
            return true;
        } catch (PDOException $e) {
            return false;
        }
    }
}
