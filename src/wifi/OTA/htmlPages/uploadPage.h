static const char *successPage __attribute__((unused)) = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>Update Completo</title>
  <style>
    body {
      font-family: Arial, sans-serif;
      background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      min-height: 100vh;
      display: flex;
      align-items: center;
      justify-content: center;
      margin: 0;
    }
    .success {
      background: white;
      padding: 40px;
      border-radius: 20px;
      text-align: center;
      box-shadow: 0 20px 60px rgba(0,0,0,0.3);
    }
    .emoji { font-size: 64px; margin-bottom: 20px; }
    h1 { color: #28a745; margin-bottom: 10px; }
    p { color: #666; }
  </style>
  <script>
    setTimeout(() => window.location.href = '/', 10000);
  </script>
</head>
<body>
  <div class="success">
    <div class="emoji">✅</div>
    <h1>Update Completo!</h1>
    <p>O dispositivo está a reiniciar...</p>
    <p style="margin-top: 20px; font-size: 14px;">A redirecionar em 10 segundos...</p>
  </div>
</body>
</html>
)rawliteral";