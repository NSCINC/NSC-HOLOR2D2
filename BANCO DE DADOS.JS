const sqlite3 = require('sqlite3').verbose();

function checkSubscription(userId) {
    return new Promise((resolve, reject) => {
        const db = new sqlite3.Database('users.db');
        
        db.get("SELECT isSubscriber FROM users WHERE id = ?", [userId], (err, row) => {
            if (err) {
                reject(err);
            } else {
                resolve(row ? row.isSubscriber : false);
            }
        });

        db.close();
    });
}

// Exemplo de uso
checkSubscription('user123').then(isSubscriber => {
    console.log(`User user123 is subscriber: ${isSubscriber}`);
}).catch(err => {
    console.error(err);
});
