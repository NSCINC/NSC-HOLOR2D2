// Definição da classe NSCABC
public class NSCABC {
    private String modelName;
    private String version;

    // Construtor para inicializar o modelo
    public NSCABC(String modelName, String version) {
        this.modelName = modelName;
        this.version = version;
    }

    // Método para gerar saída baseada em uma string de entrada
    public void generate(String input) {
        String output = String.format("Generated output based on: %s", input);
        System.out.println(output);
    }

    // Método para exibir o diálogo com o modelo
    public void dialog() {
        System.out.printf("Dialog with NSC_ABC model %s version %s\n", modelName, version);
    }

    // Método principal para executar o código
    public static void main(String[] args) {
        // Inicializando o modelo NSC_ABC
        NSCABC nscModel = new NSCABC("NSC-ABC", "1.0");

        // Exibindo o diálogo
        nscModel.dialog();

        // Gerando saída
        nscModel.generate("Teste de entrada");
    }
}
