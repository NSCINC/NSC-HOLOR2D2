// server.js

const express = require('express');
const { Pool } = require('pg');
const cors = require('cors');

const app = express();
const port = 3000; // You can change this port if needed

// Middleware
app.use(cors());
app.use(express.json());

// PostgreSQL connection setup
const pool = new Pool({
    user: 'your_db_user',
    host: 'localhost',
    database: 'your_database_name',
    password: 'your_db_password',
    port: 5432, // Default PostgreSQL port
});

// Endpoint to handle chat messages
app.post('/api/chat', async (req, res) => {
    const userInput = req.body.input;
    const dbSelection = req.body.dbSelection;

    // Example of how to save user input to the database
    try {
        const result = await pool.query('INSERT INTO chat_messages (message, db_selection) VALUES ($1, $2) RETURNING *', [userInput, dbSelection]);
        res.json({ message: 'Message saved', data: result.rows[0] });
    } catch (error) {
        console.error('Error saving message to database:', error);
        res.status(500).json({ error: 'Database error' });
    }
});

// Start the server
app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});
