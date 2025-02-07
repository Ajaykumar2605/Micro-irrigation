const char login_code[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Login</title>
  <style>
    /* Basic styling for the login page */
    body {
      font-family: Arial, sans-serif;
      background-color: #f0f8f4;
      margin: 0;
      padding: 0;
    }

    .login-container {
      max-width: 400px;
      margin: 100px auto;
      padding: 20px;
      background-color: #ffffff;
      border-radius: 8px;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
      text-align: center;
    }

    h2 {
      color: #4caf50;
      margin-bottom: 30px;
    }

    .input-group {
      margin-bottom: 15px;
      text-align: left;
    }

    .input-group label {
      font-size: 14px;
      color: #333;
    }

    .input-group input {
      width: 100%;
      padding: 10px;
      margin-top: 5px;
      font-size: 16px;
      border: 1px solid #ddd;
      border-radius: 4px;
    }

    .btn {
      background-color: #4caf50;
      color: white;
      padding: 12px 20px;
      border: none;
      border-radius: 4px;
      cursor: pointer;
      width: 100%;
      font-size: 16px;
    }

    .btn:hover {
      background-color: #45a049;
    }

    .error-message {
      color: red;
      font-size: 14px;
      margin-top: 15px;
    }
  </style>
</head>
<body>
  <div class="login-container">
    <h2>Management System<br>Login</h2>
    <form id="loginForm">
      <div class="input-group">
        <label for="user-id">User ID</label>
        <input type="text" id="user-id" name="user-id" required>
      </div>
      <div class="input-group">
        <label for="password">Password</label>
        <input type="password" id="password" name="password" required>
      </div>
      <button type="submit" class="btn">Login</button>
    </form>
    <div id="error-message" class="error-message"></div>
  </div>

  <script>
    document.getElementById('loginForm').addEventListener('submit', function(event) {
      event.preventDefault(); // Prevent form submission

      // Get the values of the user ID and password
      const userId = document.getElementById('user-id').value;
      const password = document.getElementById('password').value;

      //credentials for validation
      const validUserId = "admin";
      const validPassword = "123";

      // Get IP Address from the server
      const deviceIpAddress = "%IP_ADDRESS%";

      // Check if the entered credentials are correct
      if (userId === validUserId && password === validPassword) {
        alert("Login successful!");
        // Redirect to the dashboard using the device's IP address
        window.location.href = `http://${deviceIpAddress}/index`; 
      } else {
        alert("Invalid User ID or Password. Please try again.");
        document.getElementById('error-message').textContent = "Invalid User ID or Password. Please try again.";
      }
    });
  </script>
</body>
</html>
)rawliteral";
