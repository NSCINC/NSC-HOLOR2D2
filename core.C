#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Constantes
#define NUM_INPUT 10         // Número de entradas
#define NUM_HIDDEN 20        // Número de neurônios na camada oculta
#define NUM_OUTPUT 1         // Número de saídas
#define MAX_EPOCHS 1000      // Máximo de épocas de treinamento
#define ERROR_THRESHOLD 0.01 // Limite de erro para treinamento

typedef struct {
    double weightsInputHidden[NUM_INPUT][NUM_HIDDEN];
    double weightsHiddenOutput[NUM_HIDDEN][NUM_OUTPUT];
    double hiddenLayer[NUM_HIDDEN];
    double outputLayer[NUM_OUTPUT];
    double learningRate;
} NeuralNetwork;

// Função de ativação Sigmoid
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivada da função Sigmoid
double sigmoidDerivative(double x) {
    return x * (1.0 - x);
}

// Inicializa a rede neural com pesos aleatórios
void initializeNetwork(NeuralNetwork* nn) {
    for (int i = 0; i < NUM_INPUT; i++) {
        for (int j = 0; j < NUM_HIDDEN; j++) {
            nn->weightsInputHidden[i][j] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Peso aleatório entre -1 e 1
        }
    }
    for (int j = 0; j < NUM_HIDDEN; j++) {
        for (int k = 0; k < NUM_OUTPUT; k++) {
            nn->weightsHiddenOutput[j][k] = ((double)rand() / RAND_MAX) * 2.0 - 1.0; // Peso aleatório entre -1 e 1
        }
    }
}

// Função de treinamento da rede neural
void train(NeuralNetwork* nn, double input[][NUM_INPUT], double output[][NUM_OUTPUT], int numSamples) {
    for (int epoch = 0; epoch < MAX_EPOCHS; epoch++) {
        double totalError = 0.0;
        for (int s = 0; s < numSamples; s++) {
            // Feedforward
            for (int j = 0; j < NUM_HIDDEN; j++) {
                double activation = 0.0;
                for (int i = 0; i < NUM_INPUT; i++) {
                    activation += input[s][i] * nn->weightsInputHidden[i][j];
                }
                nn->hiddenLayer[j] = sigmoid(activation);
            }

            for (int k = 0; k < NUM_OUTPUT; k++) {
                double activation = 0.0;
                for (int j = 0; j < NUM_HIDDEN; j++) {
                    activation += nn->hiddenLayer[j] * nn->weightsHiddenOutput[j][k];
                }
                nn->outputLayer[k] = sigmoid(activation);
            }

            // Cálculo do erro
            double error = output[s][0] - nn->outputLayer[0];
            totalError += error * error;

            // Backpropagation
            double outputDelta = error * sigmoidDerivative(nn->outputLayer[0]);
            for (int j = 0; j < NUM_HIDDEN; j++) {
                nn->weightsHiddenOutput[j][0] += nn->learningRate * outputDelta * nn->hiddenLayer[j];
            }

            for (int j = 0; j < NUM_HIDDEN; j++) {
                double hiddenDelta = outputDelta * nn->weightsHiddenOutput[j][0] * sigmoidDerivative(nn->hiddenLayer[j]);
                for (int i = 0; i < NUM_INPUT; i++) {
                    nn->weightsInputHidden[i][j] += nn->learningRate * hiddenDelta * input[s][i];
                }
            }
        }

        totalError /= numSamples;
        if (totalError < ERROR_THRESHOLD) {
            printf("Treinamento interrompido precocemente na época %d\n", epoch);
            break;
        }
    }
}

// Função para prever com a rede neural treinada
void predict(NeuralNetwork* nn, double input[]) {
    for (int j = 0; j < NUM_HIDDEN; j++) {
        double activation = 0.0;
        for (int i = 0; i < NUM_INPUT; i++) {
            activation += input[i] * nn->weightsInputHidden[i][j];
        }
        nn->hiddenLayer[j] = sigmoid(activation);
    }

    for (int k = 0; k < NUM_OUTPUT; k++) {
        double activation = 0.0;
        for (int j = 0; j < NUM_HIDDEN; j++) {
            activation += nn->hiddenLayer[j] * nn->weightsHiddenOutput[j][k];
        }
        nn->outputLayer[k] = sigmoid(activation);
    }

    printf("Previsão: %f\n", nn->outputLayer[0]);
}

// Função principal
int main() {
    NeuralNetwork nn = {
        .learningRate = 0.01
    };
    initializeNetwork(&nn);

    // Dados de treinamento fictícios
    double input[2][NUM_INPUT] = {
        {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}
    };
    double output[2][NUM_OUTPUT] = {
        {1.0},
        {1.0}
    };

    // Treinar a rede neural
    train(&nn, input, output, 2);

    // Realizar previsões
    double newInput[NUM_INPUT] = {1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
    predict(&nn, newInput);

    return 0;
}
