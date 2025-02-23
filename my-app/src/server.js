const express = require('express');
const mysql = require('mysql');
const cors = require('cors');
const bodyParser = require('body-parser');

const app = express();
app.use(cors());
app.use(bodyParser.json());

// Connect to MySQL
const db = mysql.createConnection({
    host: 'localhost',
    user: 'your_mysql_user',
    password: 'your_mysql_password',
    database: 'your_database'
});

db.connect(err => {
    if (err) throw err;
    console.log('MySQL Connected...');
});

// API to save data
app.post('/save-data', (req, res) => {
    const { data } = req.body;
    const lines = data.split('\n');

    const query = 'INSERT INTO sound_data (raw_text) VALUES ?';
    const values = lines.map(line => [line]);

    db.query(query, [values], (err, result) => {
        if (err) {
            console.error(err);
            return res.status(500).send('Error saving data');
        }
        res.send('Data saved successfully');
    });
});

app.listen(5000, () => console.log('Server running on port 5000'));
