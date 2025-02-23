### For Python
#pip install -r requirements.txt


### For Web
npx create-react-app my-app  

cd my-app

npm install recharts

npm install --save react-icons

npm install @mui/material @emotion/react @emotion/styled

npm install --save material-ui

npm install motion

npm install lucide-react

### Set Up a Node.js + Express Backend

npm init -y

npm install express mysql cors body-parser dotenv bcryptjs jsonwebtoken


### 1. Fetch the Data in React
   
You need to fetch the file content from GitHub and send it to your backend for database insertion.

### 3. Create a Backend API (Node.js + Express)

Since React is frontend-only, you need a backend (e.g., Node.js + Express) to handle MySQL interactions.

#Start MySQL & Apache (phpMyAdmin at http://localhost/phpmyadmin/).
#Run Node.js backend

'node server.js
'npm start

### 4. Insert Data into MySQL

Your backend will parse the data and insert it into MySQL

CREATE TABLE sound_data ( 
    id INT AUTO_INCREMENT PRIMARY KEY,
    raw_text TEXT
);


