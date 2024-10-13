# Capacidade inicial
INITIAL_CAPACITY = 10

# Classe para o plano de investimento
class InvestmentPlan:
    def __init__(self, plan_name, initial_investment, monthly_return, annual_return, net_annual_return, slots):
        self.plan_name = plan_name
        self.initial_investment = initial_investment
        self.monthly_return = monthly_return
        self.annual_return = annual_return
        self.net_annual_return = net_annual_return
        self.slots = slots

# Gerenciador de planos de investimento
class InvestmentPlanManager:
    def __init__(self):
        self.plans = []
        self.count = 0
        self.capacity = INITIAL_CAPACITY

    # Função para adicionar um plano de investimento
    def add_investment_plan(self, plan_name, initial_investment, monthly_return, annual_return, net_annual_return, slots):
        # Verifica se é necessário aumentar a capacidade
        if self.count >= self.capacity:
            self.capacity *= 2  # Dobra a capacidade

        # Adiciona o novo plano
        new_plan = InvestmentPlan(plan_name, initial_investment, monthly_return, annual_return, net_annual_return, slots)
        self.plans.append(new_plan)  # Usa append para adicionar o novo plano
        self.count += 1  # Incrementa a contagem de planos

    # Função para exibir os planos de investimento
    def display_investment_plans(self):
        for plan in self.plans:
            print(f"Plano: {plan.plan_name}")
            print(f"Investimento Inicial: {plan.initial_investment}")
            print(f"Retorno Mensal: {plan.monthly_return}")
            print(f"Retorno Anual: {plan.annual_return}")
            print(f"Retorno Anual Líquido: {plan.net_annual_return}")
            print(f"Slots: {plan.slots}")
            print("--------------")  # Separador para melhor legibilidade

# Função principal
def main():
    manager = InvestmentPlanManager()

    # Adicionando alguns planos de investimento como exemplo
    manager.add_investment_plan("Plano A", 1000, 100, 1200, 1000, 5)
    manager.add_investment_plan("Plano B", 2000, 200, 2400, 2000, 10)

    # Exibir os planos de investimento
    manager.display_investment_plans()

# Chamada da função principal
if __name__ == "__main__":
    main()
