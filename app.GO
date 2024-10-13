package main

import (
    "encoding/json"
    "fmt"
    "log"
    "net/http"
    "os/exec"

    "github.com/gin-gonic/gin"
)

type Request struct {
    Message string `json:"message"`
}

type Response struct {
    Response string `json:"response"`
}

func main() {
    r := gin.Default()

    r.POST("/ask", func(c *gin.Context) {
        var req Request
        if err := c.ShouldBindJSON(&req); err != nil {
            c.JSON(http.StatusBadRequest, Response{Response: "Invalid request"})
            return
        }

        // Chamada ao Drake UNIX (substitua pelo seu comando específico)
        output, err := exec.Command("drake", "your_command_here", req.Message).Output()
        if err != nil {
            c.JSON(http.StatusInternalServerError, Response{Response: fmt.Sprintf("Erro: %v", err)})
            return
        }

        response := string(output)
        if response == "" {
            response = "Desculpe, não consegui processar sua solicitação."
        }

        c.JSON(http.StatusOK, Response{Response: response})
    })

    r.Static("/", "./") // Serve arquivos estáticos
    if err := r.Run(":8080"); err != nil {
        log.Fatal("Erro ao iniciar o servidor:", err)
    }
}
