import numpy as np

# Parâmetros da rede
NUM_INPUT = 10
NUM_HIDDEN = 20
NUM_OUTPUT = 1
MAX_EPOCHS = 1000
LEARNING_RATE = 0.01

# Funções de ativação
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

def sigmoid_derivative(x):
    return x * (1 - x)

# Inicializa a rede neural com pesos aleatórios
def initialize_network():
    weights_input_hidden = np.random.uniform(-1, 1, (NUM_INPUT, NUM_HIDDEN))
    weights_hidden_output = np.random.uniform(-1, 1, (NUM_HIDDEN, NUM_OUTPUT))
    return weights_input_hidden, weights_hidden_output

# Feedforward
def feedforward(input_data, weights_input_hidden, weights_hidden_output):
    hidden_layer = sigmoid(np.dot(input_data, weights_input_hidden))
    output_layer = sigmoid(np.dot(hidden_layer, weights_hidden_output))
    return hidden_layer, output_layer

# Treinamento da rede neural
def train(input_data, output_data):
    weights_input_hidden, weights_hidden_output = initialize_network()
    for epoch in range(MAX_EPOCHS):
        for i in range(len(input_data)):
            hidden_layer, output_layer = feedforward(input_data[i], weights_input_hidden, weights_hidden_output)
            
            # Backpropagation
            error = output_data[i] - output_layer
            output_delta = error * sigmoid_derivative(output_layer)

            # Atualizando os pesos
            weights_hidden_output += LEARNING_RATE * np.dot(hidden_layer.T.reshape(-1, 1), output_delta)
            hidden_delta = np.dot(output_delta, weights_hidden_output.T) * sigmoid_derivative(hidden_layer)
            weights_input_hidden += LEARNING_RATE * np.dot(input_data[i].reshape(-1, 1), hidden_delta)
            
    return weights_input_hidden, weights_hidden_output

# Dados fictícios de treinamento
input_data = np.array([[1, 0, 0, 0, 0, 0, 0, 0, 0, 0], [0, 1, 0, 0, 0, 0, 0, 0, 0, 0]])
output_data = np.array([[1], [1]])

# Treinar a rede neural
weights_input_hidden, weights_hidden_output = train(input_data, output_data)

# Previsão de um novo dado
new_input = np.array([1, 0, 0, 0, 0, 0, 0, 0, 0, 0])
hidden_layer, output_layer = feedforward(new_input, weights_input_hidden, weights_hidden_output)
print(f"Previsão: {output_layer}")
