static const char *mainPage __attribute__((unused)) = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>RFID Reader - Update</title>
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      padding: 20px;
    }
    .container {
      background: white;
      border-radius: 20px;
      box-shadow: 0 20px 60px rgba(0,0,0,0.3);
      max-width: 500px;
      width: 100%;
      padding: 40px;
    }
    h1 {
      color: #333;
      margin-bottom: 10px;
      font-size: 28px;
      text-align: center;
    }
    .subtitle {
      color: #666;
      text-align: center;
      margin-bottom: 30px;
      font-size: 14px;
    }
    .info-box {
      background: #f8f9fa;
      border-radius: 10px;
      padding: 20px;
      margin-bottom: 30px;
    }
    .info-row {
      display: flex;
      justify-content: space-between;
      margin-bottom: 10px;
      font-size: 14px;
    }
    .info-row:last-child { margin-bottom: 0; }
    .label { color: #666; }
    .value { 
      color: #333; 
      font-weight: 600;
      font-family: 'Courier New', monospace;
    }
    .upload-area {
      border: 2px dashed #667eea;
      border-radius: 10px;
      padding: 30px;
      text-align: center;
      margin-bottom: 20px;
      background: #f8f9ff;
      transition: all 0.3s;
    }
    .upload-area:hover {
      border-color: #764ba2;
      background: #f0f2ff;
    }
    input[type="file"] {
      display: none;
    }
    .file-label {
      cursor: pointer;
      color: #667eea;
      font-weight: 600;
      font-size: 16px;
    }
    .file-label:hover { color: #764ba2; }
    .file-name {
      margin-top: 15px;
      color: #333;
      font-size: 14px;
      word-break: break-all;
    }
    button {
      width: 100%;
      padding: 15px;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      color: white;
      border: none;
      border-radius: 10px;
      font-size: 16px;
      font-weight: 600;
      cursor: pointer;
      transition: transform 0.2s, box-shadow 0.2s;
    }
    button:hover {
      transform: translateY(-2px);
      box-shadow: 0 10px 20px rgba(102, 126, 234, 0.4);
    }
    button:active { transform: translateY(0); }
    button:disabled {
      background: #ccc;
      cursor: not-allowed;
      transform: none;
    }
    .progress-container {
      display: none;
      margin-top: 20px;
    }
    .progress-bar {
      width: 100%;
      height: 30px;
      background: #f0f0f0;
      border-radius: 15px;
      overflow: hidden;
    }
    .progress-fill {
      height: 100%;
      background: linear-gradient(90deg, #667eea 0%, #764ba2 100%);
      width: 0%;
      transition: width 0.3s;
      display: flex;
      align-items: center;
      justify-content: center;
      color: white;
      font-weight: 600;
      font-size: 14px;
    }
    .status {
      text-align: center;
      margin-top: 15px;
      font-size: 14px;
      color: #666;
    }
    .emoji { font-size: 48px; margin-bottom: 20px; }
  </style>
</head>
<body>
  <div class="container">
    <div class="emoji">📡</div>
    <h1>RFID Reader</h1>
    <div class="subtitle">Firmware Update System</div>
    
    <div class="info-box">
      <div class="info-row">
        <span class="label">Estado:</span>
        <span class="value" style="color: #28a745;">🟢 Online</span>
      </div>
      <div class="info-row">
        <span class="label">IP:</span>
        <span class="value">%IP%</span>
      </div>
      <div class="info-row">
        <span class="label">Free Heap:</span>
        <span class="value">%HEAP% KB</span>
      </div>
      <div class="info-row">
        <span class="label">Uptime:</span>
        <span class="value">%UPTIME%</span>
      </div>
    </div>

    <form id="uploadForm" method="POST" action="/update" enctype="multipart/form-data">
      <div class="upload-area" id="uploadArea">
        <label for="fileInput" class="file-label">
          📁 Escolher ficheiro .bin
        </label>
        <input type="file" id="fileInput" name="update" accept=".bin">
        <div class="file-name" id="fileName"></div>
      </div>
      
      <button type="submit" id="uploadBtn" disabled>🚀 Fazer Upload</button>
    </form>

    <div class="progress-container" id="progressContainer">
      <div class="progress-bar">
        <div class="progress-fill" id="progressFill">0%</div>
      </div>
      <div class="status" id="status"></div>
    </div>
  </div>

  <script>
    const fileInput = document.getElementById('fileInput');
    const fileName = document.getElementById('fileName');
    const uploadBtn = document.getElementById('uploadBtn');
    const uploadForm = document.getElementById('uploadForm');
    const progressContainer = document.getElementById('progressContainer');
    const progressFill = document.getElementById('progressFill');
    const status = document.getElementById('status');
    const uploadArea = document.getElementById('uploadArea');

    // Drag and drop
    uploadArea.addEventListener('dragover', (e) => {
      e.preventDefault();
      uploadArea.style.borderColor = '#764ba2';
      uploadArea.style.background = '#f0f2ff';
    });

    uploadArea.addEventListener('dragleave', () => {
      uploadArea.style.borderColor = '#667eea';
      uploadArea.style.background = '#f8f9ff';
    });

    uploadArea.addEventListener('drop', (e) => {
      e.preventDefault();
      uploadArea.style.borderColor = '#667eea';
      uploadArea.style.background = '#f8f9ff';
      const files = e.dataTransfer.files;
      if (files.length > 0) {
        fileInput.files = files;
        handleFileSelect();
      }
    });

    fileInput.addEventListener('change', handleFileSelect);

    function handleFileSelect() {
      const file = fileInput.files[0];
      if (file) {
        fileName.textContent = '✓ ' + file.name + ' (' + (file.size / 1024).toFixed(2) + ' KB)';
        uploadBtn.disabled = false;
      }
    }

    uploadForm.addEventListener('submit', async (e) => {
      e.preventDefault();
      
      const formData = new FormData(uploadForm);
      uploadBtn.disabled = true;
      progressContainer.style.display = 'block';
      status.textContent = '⏳ A fazer upload...';

      try {
        const xhr = new XMLHttpRequest();
        
        xhr.upload.addEventListener('progress', (e) => {
          if (e.lengthComputable) {
            const percent = Math.round((e.loaded / e.total) * 100);
            progressFill.style.width = percent + '%';
            progressFill.textContent = percent + '%';
          }
        });

        xhr.addEventListener('load', () => {
          if (xhr.status === 200) {
            status.textContent = '✅ Upload completo! A reiniciar...';
            progressFill.style.width = '100%';
            progressFill.textContent = '100%';
            setTimeout(() => {
              status.textContent = '🔄 Dispositivo a reiniciar. Aguarde 10 segundos...';
              setTimeout(() => location.reload(), 10000);
            }, 1000);
          } else {
            status.textContent = '❌ Erro no upload!';
            uploadBtn.disabled = false;
          }
        });

        xhr.addEventListener('error', () => {
          status.textContent = '❌ Erro de conexão!';
          uploadBtn.disabled = false;
        });

        xhr.open('POST', '/update');
        xhr.send(formData);
        
      } catch (error) {
        status.textContent = '❌ Erro: ' + error.message;
        uploadBtn.disabled = false;
      }
    });
  </script>
</body>
</html>
)rawliteral";