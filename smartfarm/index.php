<!DOCTYPE html>
<html lang="id">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>🌱 FAST</title>
    <meta name="description" content="Real-time monitoring dashboard for ESP32 Smart Farm sensors">
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700;800&display=swap" rel="stylesheet">
    <script src="https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/hammerjs@2.0.8/hammer.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chartjs-plugin-zoom@2.0.1/dist/chartjs-plugin-zoom.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/xlsx@0.18.5/dist/xlsx.full.min.js"></script>
    <style>
        :root {
            --bg: #0b0f19;
            --bg-card: rgba(15, 23, 42, 0.65);
            --border: rgba(148, 163, 184, 0.08);
            --border-hover: rgba(148, 163, 184, 0.18);
            --text: #f1f5f9;
            --text-2: #94a3b8;
            --text-3: #64748b;
            --green: #22c55e;
            --yellow: #eab308;
            --red: #ef4444;
            --blue: #3b82f6;
            --cyan: #06b6d4;
            --orange: #f97316;
            --purple: #a855f7;
            --radius: 14px;
            --radius-sm: 10px;
        }

        * { margin: 0; padding: 0; box-sizing: border-box; }

        body {
            font-family: 'Inter', -apple-system, sans-serif;
            background: var(--bg);
            color: var(--text);
            min-height: 100vh;
            line-height: 1.5;
        }

        /* Subtle grid background */
        body::before {
            content: '';
            position: fixed; top: 0; left: 0;
            width: 100%; height: 100%;
            background-image: radial-gradient(rgba(148,163,184,0.03) 1px, transparent 1px);
            background-size: 32px 32px;
            pointer-events: none; z-index: 0;
        }

        .app {
            position: relative; z-index: 1;
            max-width: 1200px;
            margin: 0 auto;
            padding: 20px;
        }

        /* ===== HEADER ===== */
        .header {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 28px;
            flex-wrap: wrap;
            gap: 12px;
        }

        .brand {
            display: flex; align-items: center; gap: 12px;
        }
        .brand-icon {
            width: 40px; height: 40px;
            background: linear-gradient(135deg, #22c55e, #16a34a);
            border-radius: 12px;
            display: flex; align-items: center; justify-content: center;
            font-size: 20px;
        }
        .brand h1 {
            font-size: 1.2rem; font-weight: 700;
            color: var(--text);
        }
        .brand p {
            font-size: 0.7rem; color: var(--text-3);
        }

        .header-meta {
            display: flex; align-items: center; gap: 10px;
        }

        .status-pill {
            display: flex; align-items: center; gap: 6px;
            padding: 6px 14px;
            border-radius: 50px;
            font-size: 0.72rem; font-weight: 600;
        }
        .status-pill.online {
            background: rgba(34, 197, 94, 0.1);
            color: var(--green);
        }
        .status-pill.offline {
            background: rgba(239, 68, 68, 0.1);
            color: var(--red);
        }
        .status-dot {
            width: 6px; height: 6px;
            border-radius: 50%;
            background: currentColor;
            animation: blink 2s ease-in-out infinite;
        }
        .status-pill.offline .status-dot { animation: none; }

        @keyframes blink {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.3; }
        }

        .clock {
            font-size: 0.72rem;
            color: var(--text-3);
            padding: 6px 12px;
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 8px;
            font-variant-numeric: tabular-nums;
        }

        /* ===== CARDS GRID ===== */
        .cards {
            display: grid;
            grid-template-columns: repeat(3, 1fr);
            gap: 14px;
            margin-bottom: 14px;
        }

        .card {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            padding: 18px 20px;
            backdrop-filter: blur(12px);
            transition: border-color 0.2s;
        }
        .card:hover { border-color: var(--border-hover); }

        .card-top {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 10px;
        }
        .card-label {
            font-size: 0.68rem;
            font-weight: 600;
            color: var(--text-3);
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        .card-icon {
            font-size: 18px;
        }

        .card-value {
            font-size: 2.2rem;
            font-weight: 800;
            letter-spacing: -1px;
            line-height: 1;
            margin-bottom: 8px;
        }
        .card-unit {
            font-size: 0.9rem;
            font-weight: 400;
            opacity: 0.5;
        }

        .card.temp .card-value { color: var(--orange); }
        .card.hum .card-value { color: var(--blue); }
        .card.mois .card-value { color: var(--cyan); }

        /* Condition Badge */
        .condition {
            display: inline-flex;
            align-items: center;
            gap: 5px;
            padding: 4px 10px;
            border-radius: 6px;
            font-size: 0.68rem;
            font-weight: 600;
        }
        .condition.good {
            background: rgba(34, 197, 94, 0.1);
            color: var(--green);
        }
        .condition.warn {
            background: rgba(234, 179, 8, 0.1);
            color: var(--yellow);
        }
        .condition.danger {
            background: rgba(239, 68, 68, 0.1);
            color: var(--red);
        }
        .condition.cold {
            background: rgba(59, 130, 246, 0.1);
            color: var(--blue);
        }

        /* ===== MOISTURE DETAIL CHIPS ===== */
        .mois-grid {
            display: grid;
            grid-template-columns: repeat(4, 1fr);
            gap: 10px;
            margin-bottom: 24px;
        }

        .mois-chip {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            padding: 14px;
            backdrop-filter: blur(12px);
            text-align: center;
            transition: border-color 0.2s;
        }
        .mois-chip:hover { border-color: var(--border-hover); }

        .mois-chip-label {
            font-size: 0.65rem;
            color: var(--text-3);
            text-transform: uppercase;
            letter-spacing: 0.3px;
            margin-bottom: 4px;
        }
        .mois-chip-value {
            font-size: 1.3rem;
            font-weight: 700;
            color: var(--cyan);
            margin-bottom: 4px;
        }
        .mois-chip .condition {
            font-size: 0.6rem;
            padding: 2px 7px;
        }

        /* ===== SECTION LABEL ===== */
        .section-label {
            font-size: 0.72rem;
            font-weight: 600;
            color: var(--text-3);
            text-transform: uppercase;
            letter-spacing: 0.8px;
            margin-bottom: 12px;
            display: flex;
            align-items: center;
            gap: 8px;
        }
        .section-label::after {
            content: '';
            flex: 1;
            height: 1px;
            background: var(--border);
        }

        /* ===== FILTER TABS ===== */
        .filter-row {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 16px;
            flex-wrap: wrap;
            gap: 10px;
        }

        .tabs {
            display: flex;
            gap: 4px;
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: 10px;
            padding: 3px;
        }
        .tab {
            padding: 7px 18px;
            border: none;
            border-radius: 8px;
            background: transparent;
            color: var(--text-3);
            font-family: inherit;
            font-size: 0.75rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.2s;
        }
        .tab:hover { color: var(--text-2); }
        .tab.active {
            background: rgba(59, 130, 246, 0.15);
            color: var(--blue);
        }

        .filter-meta {
            display: flex; align-items: center; gap: 8px;
        }
        .badge {
            font-size: 0.68rem;
            color: var(--green);
            background: rgba(34, 197, 94, 0.08);
            padding: 4px 10px;
            border-radius: 6px;
            font-variant-numeric: tabular-nums;
        }

        .spinner {
            width: 16px; height: 16px;
            border: 2px solid var(--border);
            border-top-color: var(--blue);
            border-radius: 50%;
            animation: spin 0.6s linear infinite;
            display: none;
        }
        .spinner.on { display: block; }
        @keyframes spin { to { transform: rotate(360deg); } }

        /* ===== CHART CARD ===== */
        .chart-card {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            padding: 20px;
            margin-bottom: 14px;
            backdrop-filter: blur(12px);
        }

        .chart-head {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 4px;
        }
        .chart-title {
            font-size: 0.82rem;
            font-weight: 600;
            color: var(--text-2);
            display: flex; align-items: center; gap: 6px;
        }
        .chart-hint {
            font-size: 0.6rem;
            color: var(--text-3);
            opacity: 0.6;
            margin-bottom: 8px;
        }
        .chart-container {
            position: relative;
            width: 100%;
            height: 240px;
        }

        /* ===== DOWNLOAD SECTION ===== */
        .dl-section {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            padding: 20px;
            margin-bottom: 14px;
        }

        .dl-section .section-label {
            margin-bottom: 4px;
        }
        .dl-sub {
            font-size: 0.65rem;
            color: var(--text-3);
            margin-bottom: 12px;
        }

        .dl-grid {
            display: grid;
            grid-template-columns: repeat(4, 1fr);
            gap: 8px;
        }

        .dl-btn {
            display: flex; align-items: center; justify-content: center; gap: 6px;
            padding: 10px 14px;
            background: rgba(34, 197, 94, 0.06);
            border: 1px solid rgba(34, 197, 94, 0.15);
            border-radius: var(--radius-sm);
            color: var(--green);
            font-family: inherit;
            font-size: 0.75rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.2s;
        }
        .dl-btn:hover {
            background: rgba(34, 197, 94, 0.12);
            border-color: rgba(34, 197, 94, 0.3);
            transform: translateY(-1px);
        }
        .dl-btn:active { transform: translateY(0); }
        .dl-btn.loading { opacity: 0.5; pointer-events: none; }
        .dl-btn .dl-spin {
            width: 12px; height: 12px;
            border: 2px solid rgba(34,197,94,0.2);
            border-top-color: var(--green);
            border-radius: 50%;
            animation: spin 0.6s linear infinite;
            display: none;
        }
        .dl-btn.loading .dl-spin { display: inline-block; }
        .dl-btn.loading .dl-icon { display: none; }

        /* ===== TABLE ===== */
        .table-card {
            background: var(--bg-card);
            border: 1px solid var(--border);
            border-radius: var(--radius);
            padding: 20px;
            margin-bottom: 14px;
        }

        .table-head {
            display: flex;
            align-items: center;
            justify-content: space-between;
            margin-bottom: 12px;
        }

        .table-wrap {
            overflow-x: auto;
            max-height: 300px;
            overflow-y: auto;
        }
        .table-wrap::-webkit-scrollbar { width: 4px; height: 4px; }
        .table-wrap::-webkit-scrollbar-thumb {
            background: rgba(255,255,255,0.08);
            border-radius: 2px;
        }

        table {
            width: 100%;
            border-collapse: collapse;
            font-size: 0.72rem;
        }
        table th {
            padding: 8px 12px;
            text-align: left;
            font-weight: 600;
            color: var(--text-3);
            text-transform: uppercase;
            letter-spacing: 0.4px;
            font-size: 0.62rem;
            border-bottom: 1px solid var(--border);
            white-space: nowrap;
        }
        table td {
            padding: 7px 12px;
            color: var(--text-2);
            font-variant-numeric: tabular-nums;
            border-bottom: 1px solid rgba(255,255,255,0.02);
            white-space: nowrap;
        }
        table tr:hover td {
            background: rgba(59, 130, 246, 0.03);
        }
        table tr.latest td {
            color: var(--text);
            background: rgba(59, 130, 246, 0.05);
        }
        .cond-dot {
            display: inline-block;
            width: 6px; height: 6px;
            border-radius: 50%;
            margin-right: 4px;
        }
        .cond-dot.g { background: var(--green); }
        .cond-dot.w { background: var(--yellow); }
        .cond-dot.d { background: var(--red); }
        .cond-dot.c { background: var(--blue); }

        /* ===== FOOTER ===== */
        .footer {
            text-align: center;
            padding: 20px;
            font-size: 0.65rem;
            color: var(--text-3);
        }

        /* ===== RESPONSIVE ===== */
        @media (max-width: 768px) {
            .app { padding: 14px; }
            .cards { grid-template-columns: 1fr; }
            .mois-grid { grid-template-columns: repeat(2, 1fr); }
            .dl-grid { grid-template-columns: repeat(2, 1fr); }
            .card-value { font-size: 1.8rem; }
            .chart-container { height: 200px; }
            .header { flex-direction: column; align-items: flex-start; }
        }

        @media (max-width: 480px) {
            .mois-grid { grid-template-columns: repeat(2, 1fr); }
            .dl-grid { grid-template-columns: 1fr 1fr; }
            .tabs { flex-wrap: wrap; }
        }

        /* ===== FADE IN ===== */
        .card, .mois-chip, .chart-card, .dl-section, .table-card {
            animation: fadeIn 0.4s ease both;
        }
        @keyframes fadeIn {
            from { opacity: 0; transform: translateY(8px); }
            to { opacity: 1; transform: translateY(0); }
        }

        /* Flash update */
        .flash { animation: flash 0.5s ease-out; }
        @keyframes flash {
            0% { text-shadow: 0 0 8px rgba(59,130,246,0.5); }
            100% { text-shadow: none; }
        }
    </style>
</head>
<body>
    <div class="app">
        <!-- HEADER -->
        <header class="header" id="header">
            <div class="brand">
                <div class="brand-icon">🌱</div>
                <div>
                    <h1>FAST</h1>
                    <p>FNC Comp — Main Sensor</p>
                </div>
            </div>
            <div class="header-meta">
                <div class="status-pill offline" id="statusPill">
                    <span class="status-dot"></span>
                    <span id="statusText">Offline</span>
                </div>
                <div class="clock" id="clock">--:--:--</div>
            </div>
        </header>

        <!-- MAIN METRIC CARDS -->
        <div class="cards">
            <div class="card temp">
                <div class="card-top">
                    <span class="card-label">Suhu</span>
                    <span class="card-icon">🌡️</span>
                </div>
                <div class="card-value" id="v-temp">--<span class="card-unit">°C</span></div>
                <div class="condition" id="c-temp">—</div>
            </div>

            <div class="card hum">
                <div class="card-top">
                    <span class="card-label">Kelembapan Udara</span>
                    <span class="card-icon">💨</span>
                </div>
                <div class="card-value" id="v-hum">--<span class="card-unit">%</span></div>
                <div class="condition" id="c-hum">—</div>
            </div>

            <div class="card mois">
                <div class="card-top">
                    <span class="card-label">Kelembapan Tanah (Avg)</span>
                    <span class="card-icon">💧</span>
                </div>
                <div class="card-value" id="v-mois">--<span class="card-unit">%</span></div>
                <div class="condition" id="c-mois">—</div>
            </div>
        </div>

        <!-- MOISTURE SENSORS -->
        <div class="mois-grid">
            <div class="mois-chip">
                <div class="mois-chip-label">Sensor 1</div>
                <div class="mois-chip-value" id="v-m1">--%</div>
                <div class="condition" id="c-m1">—</div>
            </div>
            <div class="mois-chip">
                <div class="mois-chip-label">Sensor 2</div>
                <div class="mois-chip-value" id="v-m2">--%</div>
                <div class="condition" id="c-m2">—</div>
            </div>
            <div class="mois-chip">
                <div class="mois-chip-label">Sensor 3</div>
                <div class="mois-chip-value" id="v-m3">--%</div>
                <div class="condition" id="c-m3">—</div>
            </div>
            <div class="mois-chip">
                <div class="mois-chip-label">Sensor 4</div>
                <div class="mois-chip-value" id="v-m4">--%</div>
                <div class="condition" id="c-m4">—</div>
            </div>
        </div>

        <!-- CHART FILTER -->
        <div class="section-label">📊 Grafik Sensor</div>
        <div class="filter-row">
            <div class="tabs" id="tabs">
                <button class="tab active" data-range="day">1 Hari</button>
                <button class="tab" data-range="week">1 Minggu</button>
                <button class="tab" data-range="month">1 Bulan</button>
                <button class="tab" data-range="all">Semua</button>
            </div>
            <div class="filter-meta">
                <div class="spinner" id="spinner"></div>
                <div class="badge" id="dataCount">—</div>
            </div>
        </div>

        <!-- CHARTS -->
        <div class="chart-card">
            <div class="chart-head">
                <div class="chart-title">🌡️ Suhu</div>
            </div>
            <div class="chart-hint">Scroll zoom ·</div>
            <div class="chart-container"><canvas id="chTemp"></canvas></div>
        </div>

        <div class="chart-card">
            <div class="chart-head">
                <div class="chart-title">💨 Kelembapan Udara</div>
            </div>
            <div class="chart-hint">Scroll zoom ·</div>
            <div class="chart-container"><canvas id="chHum"></canvas></div>
        </div>

        <div class="chart-card">
            <div class="chart-head">
                <div class="chart-title">💧 Kelembapan Tanah</div>
            </div>
            <div class="chart-hint">Scroll zoom ·</div>
            <div class="chart-container" style="height:280px;"><canvas id="chMois"></canvas></div>
        </div>

        <!-- DOWNLOAD -->
        <div class="dl-section">
            <div class="section-label">📥 Download Excel</div>
            <div class="dl-sub">Export data sensor dalam format .xlsx</div>
            <div class="dl-grid">
                <button class="dl-btn" id="dl-day" onclick="downloadExcel('day')">
                    <span class="dl-icon">📅</span><span class="dl-spin"></span> 1 Hari
                </button>
                <button class="dl-btn" id="dl-week" onclick="downloadExcel('week')">
                    <span class="dl-icon">📅</span><span class="dl-spin"></span> 1 Minggu
                </button>
                <button class="dl-btn" id="dl-month" onclick="downloadExcel('month')">
                    <span class="dl-icon">📅</span><span class="dl-spin"></span> 1 Bulan
                </button>
                <button class="dl-btn" id="dl-all" onclick="downloadExcel('all')">
                    <span class="dl-icon">📅</span><span class="dl-spin"></span> Semua
                </button>
            </div>
        </div>

        <!-- TABLE -->
        <div class="table-card">
            <div class="table-head">
                <div class="chart-title">📋 Data Terbaru</div>
                <div class="badge" id="tblCount">—</div>
            </div>
            <div class="table-wrap">
                <table id="dataTable">
                    <thead>
                        <tr>
                            <th>Waktu</th>
                            <th>Suhu</th>
                            <th>Kondisi</th>
                            <th>Humidity</th>
                            <th>Kondisi</th>
                            <th>M.Avg</th>
                            <th>Kondisi</th>
                            <th>M1</th>
                            <th>M2</th>
                            <th>M3</th>
                            <th>M4</th>
                        </tr>
                    </thead>
                    <tbody id="tblBody">
                        <tr><td colspan="11" style="text-align:center;padding:30px;color:var(--text-3);">Memuat data…</td></tr>
                    </tbody>
                </table>
            </div>
        </div>

        <footer class="footer">🌱 Smart Farm — FNC Comp © 2026</footer>
    </div>

<script>
// ============================================
// CONDITION LOGIC
// ============================================
function tempCondition(v) {
    if (v === null || isNaN(v)) return { text: '—', cls: '', dot: '' };
    if (v < 15) return { text: '❄️ Terlalu Dingin', cls: 'cold', dot: 'c' };
    if (v < 20) return { text: '🌤️ Agak Dingin', cls: 'warn', dot: 'w' };
    if (v <= 30) return { text: '✅ Normal', cls: 'good', dot: 'g' };
    if (v <= 35) return { text: '🔥 Agak Panas', cls: 'warn', dot: 'w' };
    return { text: '🔥 Terlalu Panas', cls: 'danger', dot: 'd' };
}

function humCondition(v) {
    if (v === null || isNaN(v)) return { text: '—', cls: '', dot: '' };
    if (v < 30) return { text: '🏜️ Terlalu Kering', cls: 'danger', dot: 'd' };
    if (v < 50) return { text: '💨 Agak Kering', cls: 'warn', dot: 'w' };
    if (v <= 70) return { text: '✅ Normal', cls: 'good', dot: 'g' };
    if (v <= 85) return { text: '💦 Agak Lembap', cls: 'warn', dot: 'w' };
    return { text: '💦 Terlalu Lembap', cls: 'danger', dot: 'd' };
}

function moisCondition(v) {
    if (v === null || isNaN(v)) return { text: '—', cls: '', dot: '' };
    if (v < 20) return { text: '🏜️ Terlalu Kering', cls: 'danger', dot: 'd' };
    if (v < 40) return { text: '⚠️ Kering', cls: 'warn', dot: 'w' };
    if (v <= 70) return { text: '✅ Normal', cls: 'good', dot: 'g' };
    if (v <= 85) return { text: '💦 Basah', cls: 'warn', dot: 'w' };
    return { text: '💦 Terlalu Basah', cls: 'danger', dot: 'd' };
}

function setCondition(elId, cond) {
    const el = document.getElementById(elId);
    el.textContent = cond.text;
    el.className = 'condition ' + cond.cls;
}

// ============================================
// CHART SETUP
// ============================================
Chart.defaults.color = '#64748b';
Chart.defaults.borderColor = 'rgba(255,255,255,0.03)';
Chart.defaults.font.family = "'Inter', sans-serif";
Chart.defaults.font.size = 11;

const zoomOpts = {
    pan: { enabled: true, mode: 'x' },
    zoom: {
        wheel: { enabled: true, speed: 0.05 },
        pinch: { enabled: true },
        mode: 'x',
    },
    limits: { x: { minRange: 3 } }
};

function mkOpts(label) {
    return {
        responsive: true, maintainAspectRatio: false,
        interaction: { mode: 'index', intersect: false },
        plugins: {
            legend: { display: true, position: 'top', align: 'end',
                labels: { boxWidth: 8, boxHeight: 8, usePointStyle: true, pointStyle: 'circle', padding: 14, font: { size: 10 } }
            },
            tooltip: {
                backgroundColor: 'rgba(10,15,25,0.95)', titleColor: '#f1f5f9', bodyColor: '#94a3b8',
                borderColor: 'rgba(148,163,184,0.1)', borderWidth: 1, cornerRadius: 8, padding: 10,
                displayColors: true, boxWidth: 8, boxHeight: 8, usePointStyle: true,
            },
            zoom: zoomOpts,
        },
        scales: {
            x: { grid: { display: false }, ticks: { maxTicksLimit: 10, font: { size: 9 }, maxRotation: 40 } },
            y: { grid: { color: 'rgba(255,255,255,0.03)' }, ticks: { font: { size: 9 } },
                title: label ? { display: true, text: label, font: { size: 9 } } : undefined }
        },
        elements: {
            point: { radius: 0, hoverRadius: 4 },
            line: { tension: 0.4, borderWidth: 2 }
        },
        animation: { duration: 400 },
    };
}

const chTemp = new Chart(document.getElementById('chTemp'), {
    type: 'line',
    data: { labels: [], datasets: [{ label: 'Suhu (°C)', data: [], borderColor: '#f97316', backgroundColor: 'rgba(249,115,22,0.06)', fill: true }] },
    options: mkOpts('°C')
});

const chHum = new Chart(document.getElementById('chHum'), {
    type: 'line',
    data: { labels: [], datasets: [{ label: 'Humidity (%)', data: [], borderColor: '#3b82f6', backgroundColor: 'rgba(59,130,246,0.06)', fill: true }] },
    options: mkOpts('%')
});

const chMois = new Chart(document.getElementById('chMois'), {
    type: 'line',
    data: { labels: [], datasets: [
        { label: 'Rata-rata', data: [], borderColor: '#06b6d4', backgroundColor: 'rgba(6,182,212,0.06)', fill: true, borderWidth: 2.5 },
        { label: 'S1', data: [], borderColor: '#22d3ee', borderDash: [4,3], borderWidth: 1.2 },
        { label: 'S2', data: [], borderColor: '#38bdf8', borderDash: [4,3], borderWidth: 1.2 },
        { label: 'S3', data: [], borderColor: '#818cf8', borderDash: [4,3], borderWidth: 1.2 },
        { label: 'S4', data: [], borderColor: '#c084fc', borderDash: [4,3], borderWidth: 1.2 },
    ]},
    options: mkOpts('%')
});

// ============================================
// DATA FETCHING
// ============================================
const API = 'api/data.php';
let lastId = null, online = false, range = 'day';

async function fetchJSON(url) {
    try {
        const r = await fetch(url, { cache: 'no-store' });
        if (!r.ok) throw new Error(r.status);
        return await r.json();
    } catch(e) { console.error(e); return null; }
}

function flash(el) { el.classList.remove('flash'); void el.offsetWidth; el.classList.add('flash'); }

// --- Update latest ---
async function updateLatest() {
    const r = await fetchJSON(`${API}?type=latest`);
    if (!r?.success || !r.data) { setOffline(); return; }
    setOnline();
    const d = r.data;
    if (d.id === lastId) return;
    lastId = d.id;

    // Temperature
    const tEl = document.getElementById('v-temp');
    tEl.innerHTML = `${d.temperature}<span class="card-unit">°C</span>`;
    flash(tEl);
    setCondition('c-temp', tempCondition(parseFloat(d.temperature)));

    // Humidity
    const hEl = document.getElementById('v-hum');
    hEl.innerHTML = `${d.humidity}<span class="card-unit">%</span>`;
    flash(hEl);
    setCondition('c-hum', humCondition(parseFloat(d.humidity)));

    // Moisture avg
    const mEl = document.getElementById('v-mois');
    mEl.innerHTML = `${d.mois_avg}<span class="card-unit">%</span>`;
    flash(mEl);
    setCondition('c-mois', moisCondition(parseFloat(d.mois_avg)));

    // Individual moisture
    ['mois1','mois2','mois3','mois4'].forEach((k, i) => {
        const el = document.getElementById(`v-m${i+1}`);
        el.textContent = `${d[k]}%`;
        flash(el);
        setCondition(`c-m${i+1}`, moisCondition(parseFloat(d[k])));
    });
}

// --- Chart data ---
function fmtLabel(s, rng) {
    if (!s) return '';
    if (rng === 'all') return s.substring(0, 10);
    if (rng === 'day') return s.substring(11, 19);
    return s.substring(5, 16);
}

async function loadCharts(rng) {
    const sp = document.getElementById('spinner');
    sp.classList.add('on');
    const r = await fetchJSON(`${API}?type=chart&range=${rng}`);
    sp.classList.remove('on');
    if (!r?.success || !r.data) { document.getElementById('dataCount').textContent = '0'; return; }

    const d = r.data;
    const labels = d.map(x => fmtLabel(x.created_at, rng));
    document.getElementById('dataCount').textContent = `${d.length} data`;

    chTemp.data.labels = labels;
    chTemp.data.datasets[0].data = d.map(x => parseFloat(x.temperature));
    chTemp.resetZoom(); chTemp.update('none');

    chHum.data.labels = labels;
    chHum.data.datasets[0].data = d.map(x => parseFloat(x.humidity));
    chHum.resetZoom(); chHum.update('none');

    chMois.data.labels = labels;
    chMois.data.datasets[0].data = d.map(x => parseFloat(x.mois_avg));
    chMois.data.datasets[1].data = d.map(x => parseFloat(x.mois1));
    chMois.data.datasets[2].data = d.map(x => parseFloat(x.mois2));
    chMois.data.datasets[3].data = d.map(x => parseFloat(x.mois3));
    chMois.data.datasets[4].data = d.map(x => parseFloat(x.mois4));
    chMois.resetZoom(); chMois.update('none');
}

// --- History for table ---
async function updateTable() {
    const r = await fetchJSON(`${API}?type=history&n=60`);
    if (!r?.success) return;
    const rows = r.data.slice(-20).reverse();
    const tbody = document.getElementById('tblBody');
    document.getElementById('tblCount').textContent = `${rows.length} terbaru`;

    if (!rows.length) {
        tbody.innerHTML = '<tr><td colspan="11" style="text-align:center;padding:30px;color:var(--text-3);">Belum ada data</td></tr>';
        return;
    }

    tbody.innerHTML = rows.map((r, i) => {
        const tc = tempCondition(parseFloat(r.temperature));
        const hc = humCondition(parseFloat(r.humidity));
        const mc = moisCondition(parseFloat(r.mois_avg));
        return `<tr class="${i===0?'latest':''}">
            <td>${r.created_at.substring(11,19)}</td>
            <td>${r.temperature}°C</td>
            <td><span class="cond-dot ${tc.dot}"></span>${tc.text.replace(/[^\w\s]/g,'').trim()}</td>
            <td>${r.humidity}%</td>
            <td><span class="cond-dot ${hc.dot}"></span>${hc.text.replace(/[^\w\s]/g,'').trim()}</td>
            <td>${r.mois_avg}%</td>
            <td><span class="cond-dot ${mc.dot}"></span>${mc.text.replace(/[^\w\s]/g,'').trim()}</td>
            <td>${r.mois1}</td><td>${r.mois2}</td><td>${r.mois3}</td><td>${r.mois4}</td>
        </tr>`;
    }).join('');
}

// --- Status ---
function setOnline() { if(online) return; online=true; const p=document.getElementById('statusPill'); p.className='status-pill online'; document.getElementById('statusText').textContent='Online'; }
function setOffline() { online=false; const p=document.getElementById('statusPill'); p.className='status-pill offline'; document.getElementById('statusText').textContent='Offline'; }

// --- Clock ---
function tick() { document.getElementById('clock').textContent = new Date().toLocaleTimeString('id-ID',{hour12:false}); }

// --- Tabs ---
document.getElementById('tabs').addEventListener('click', e => {
    const t = e.target.closest('.tab');
    if (!t) return;
    document.querySelectorAll('.tab').forEach(x => x.classList.remove('active'));
    t.classList.add('active');
    range = t.dataset.range;
    loadCharts(range);
});

// --- Zoom reset ---
document.getElementById('chTemp').addEventListener('dblclick', () => chTemp.resetZoom());
document.getElementById('chHum').addEventListener('dblclick', () => chHum.resetZoom());
document.getElementById('chMois').addEventListener('dblclick', () => chMois.resetZoom());

// --- Download Excel ---
async function downloadExcel(rng) {
    const btn = document.getElementById(`dl-${rng}`);
    btn.classList.add('loading');
    try {
        const r = await fetchJSON(`${API}?type=download&range=${rng}`);
        if (!r?.success || !r.data?.length) { alert('Tidak ada data.'); btn.classList.remove('loading'); return; }

        const rows = r.data.map(d => ({
            'Waktu': d.created_at,
            'Suhu (°C)': d.temperature,
            'Kondisi Suhu': tempCondition(parseFloat(d.temperature)).text.replace(/[^\w\s]/g,'').trim(),
            'Humidity (%)': d.humidity,
            'Kondisi Humidity': humCondition(parseFloat(d.humidity)).text.replace(/[^\w\s]/g,'').trim(),
            'Moisture Avg (%)': d.mois_avg,
            'Kondisi Tanah': moisCondition(parseFloat(d.mois_avg)).text.replace(/[^\w\s]/g,'').trim(),
            'Moisture 1': d.mois1,
            'Moisture 2': d.mois2,
            'Moisture 3': d.mois3,
            'Moisture 4': d.mois4,
        }));

        const ws = XLSX.utils.json_to_sheet(rows);
        ws['!cols'] = [{wch:20},{wch:10},{wch:16},{wch:12},{wch:16},{wch:14},{wch:16},{wch:10},{wch:10},{wch:10},{wch:10}];
        const wb = XLSX.utils.book_new();
        XLSX.utils.book_append_sheet(wb, ws, 'Sensor Data');
        const names = {day:'1-Hari',week:'1-Minggu',month:'1-Bulan',all:'Semua'};
        XLSX.writeFile(wb, `SmartFarm_${names[rng]}_${new Date().toISOString().substring(0,10)}.xlsx`);
    } catch(e) { console.error(e); alert('Gagal download.'); }
    btn.classList.remove('loading');
}

// ============================================
// INIT
// ============================================
(async () => {
    tick(); setInterval(tick, 1000);
    await Promise.all([updateLatest(), loadCharts(range), updateTable()]);
    setInterval(updateLatest, 2000);
    setInterval(() => { if(range==='day') loadCharts('day'); }, 8000);
    setInterval(updateTable, 5000);
})();
</script>
</body>
</html>
