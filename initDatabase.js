const sqlite3 = require('sqlite3').verbose();
const db = new sqlite3.Database('users.db');

db.serialize(() => {
    // Criação da tabela de usuários
    db.run(`CREATE TABLE IF NOT EXISTS users (
        id TEXT PRIMARY KEY,
        name TEXT NOT NULL,
        isSubscriber BOOLEAN NOT NULL
    )`);

    // Inserindo dados de exemplo
    const stmt = db.prepare("INSERT OR IGNORE INTO users (id, name, isSubscriber) VALUES (?, ?, ?)");
    stmt.run("user123", "Lucas", true);
    stmt.run("user456", "Ana", true);
    stmt.run("user789", "Carlos", false);
    stmt.finalize();
});

db.close();
