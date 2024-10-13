// Função para carregar usuários do arquivo JSON
async function loadUsers() {
    const response = await fetch('users.json');
    const users = await response.json();
    return users;
}

// Função para verificar se o usuário é assinante
async function checkSubscription(userId) {
    const users = await loadUsers();
    const user = users.find(user => user.id === userId);
    return user ? user.isSubscriber : false;
}

// Exemplo de uso
(async () => {
    const userId = 'user123';
    const isSubscriber = await checkSubscription(userId);
    console.log(`User ${userId} is subscriber: ${isSubscriber}`);
})();
