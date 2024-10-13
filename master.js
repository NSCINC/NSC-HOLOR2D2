// master.js

// Import individual database modules
import { firestoreHandler } from './firestore.js';
import { cosmosHandler } from './cosmos.js';
import { dynamodbHandler } from './dynamodb.js';

// Main function to handle user input and call appropriate database function
async function handleUserInput(userInput, dbSelection) {
    try {
        let response;

        switch (dbSelection) {
            case 'gcp_firestore':
                response = await firestoreHandler(userInput);
                break;
            case 'azure_cosmos':
                response = await cosmosHandler(userInput);
                break;
            case 'aws_dynamodb':
                response = await dynamodbHandler(userInput);
                break;
            default:
                response = "Please select a database option.";
                break;
        }

        return response;
    } catch (error) {
        console.error('Error handling user input:', error);
        return 'An error occurred while processing your request.';
    }
}

// Function to initialize the chat and set up event listeners
function initChat() {
    const sendButton = document.getElementById('send-button'); // Update button id if needed
    const userInputField = document.getElementById('user-input');
    const dbSelectField = document.getElementById('database-select');
    
    sendButton.addEventListener('click', async () => {
        const userInput = userInputField.value.trim();
        const dbSelection = dbSelectField.value;

        if (!userInput) return;

        const botResponse = await handleUserInput(userInput, dbSelection);
        displayMessage('user', userInput);
        displayMessage('bot', botResponse);
        
        userInputField.value = '';
    });
}

// Function to display messages
function displayMessage(sender, text) {
    const messageContainer = document.getElementById('messages');
    const message = document.createElement('div');
    message.className = `message ${sender}`;
    message.textContent = text;
    messageContainer.appendChild(message);
    messageContainer.scrollTop = messageContainer.scrollHeight; // Auto-scroll
}
// master.js

async function handleUserInput(userInput, dbSelection) {
    try {
        const response = await fetch('http://localhost:3000/api/chat', {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({ input: userInput, dbSelection }),
        });

        const data = await response.json();
        return data.message; // You can adjust this based on what you want to return
    } catch (error) {
        console.error('Error handling user input:', error);
        return 'An error occurred while processing your request.';
    }
}

// Initialize the chat when the window loads
window.onload = initChat;
