# Controle de Velocidade de um Robô Micromouse

Trabalho de Conclusão de Curso (TCC) em **Engenharia Elétrica** da **Universidade Federal do Piauí (UFPI)**, Centro de Tecnologia, Teresina, 2026.

## Autoria

- **Autor da monografia:** Händel Mateus Carvalho Sarmento
- **Orientador:** Prof. Dr. Otacílio da Mota Almeida (UFPI)
- **Título:** *Controle de Velocidade de um Robô Micromouse*
- **Grau:** Bacharel em Engenharia Elétrica

A monografia contida neste repositório é de autoria de Händel Mateus Carvalho Sarmento. Ao reutilizar trechos, figuras ou resultados, cite o trabalho.

## Sobre o projeto

Os *micromouses* são robôs móveis de pequeno porte, com tração diferencial, que percorrem e resolvem labirintos em competições. Antes de seguir trajetórias e resolver o labirinto, o robô precisa controlar bem suas velocidades. Este trabalho concentra-se nessa etapa.

**Objetivo geral:** desenvolver e implementar estratégias de controle de velocidade e telemetria para um robô micromouse, visando melhorar seu desempenho de navegação e movimentação.

**Objetivos específicos:**

- Identificar e modelar o comportamento dos motores (descrição no tempo, função de transferência e equações a diferenças).
- Estudar o movimento diferencial do robô por equações cinemáticas.
- Projetar, implementar e avaliar controladores PID para os motores e para as velocidades globais (linear e angular), com malhas internas e supervisórias.
- Desenvolver comunicação sem fio para telemetria, de modo que os dados sejam coletados com o robô em movimento, sem cabos.
- Escrever em C, para o sistema embarcado, algoritmos adaptados de códigos em MATLAB.

## Resumo da abordagem

- **Plataforma:** robô micromouse com microcontrolador STM32F405RGT6 (placa uMart Lite Plus).
- **Telemetria:** o STM32 envia os dados por UART (9600 bps) a um ESP8266 acoplado ao robô; este os transmite por **ESP-NOW** a um ESP-WROOM-32 ligado ao PC por USB; no PC, o MATLAB recebe os dados.
- **Identificação:** período de amostragem de 10 ms, entrada aleatória (PRBS) de ±10 % em torno de um degrau e estimador de mínimos quadrados não recursivo, para os motores e para as velocidades linear e angular.
- **Controle:** PID por motor (malha interna) e PID supervisório para as velocidades linear e angular (malha externa), com sintonia pelo método do relé e ressintonia.
- **Avaliação:** soma do erro, variância da saída e do sinal de controle, tempo de assentamento e sobressinal.

Os resultados indicam que a telemetria funcionou de forma adequada e que os motores mantiveram velocidades estáveis e controladas para as diversas referências testadas. Os controladores das velocidades globais foram avaliados e servem de base para as etapas seguintes, como rastreio de trajetória e algoritmos de resolução de labirinto (por exemplo, *Flood Fill*).

## Estrutura do repositório

```
tcc-controle-velocidade-micromouse/
├── documento.tex              # Arquivo principal da monografia
├── elementos-pre-textuais/    # Resumo, abstract, agradecimentos, dedicatória, listas, ficha catalográfica
├── elementos-textuais/        # Introdução, fundamentação teórica, metodologia, resultados, conclusão
├── elementos-pos-textuais/    # Referências (.bib), glossário, apêndices e anexos
├── figuras/                   # Figuras por capítulo (introdução, fundamentação, metodologia, resultados)
├── lib/                       # Preâmbulo, estilo ufctex.sty e logotipos
├── DEVKIT_SEND_RECEIVE.ino    # Firmware do ESP-WROOM-32 (lado do PC)
├── WEMOS_SEND_RECEIVE.ino     # Firmware do ESP8266 (lado do robô)
├── compilado.m                # Funções MATLAB de comunicação serial com o ESP
├── Makefile                   # Compilação do documento LaTeX
├── documento.pdf              # PDF gerado da monografia
└── README.md                  # README do modelo ufctex (original)
```

Os arquivos `.aux`, `.log`, `.toc`, `.bbl` e semelhantes na raiz são gerados pela compilação do LaTeX. Os `hs_err_pid*.log` e `replay_pid*.log` são relatórios de falha da JVM e não fazem parte do projeto.

## Como compilar a monografia

O texto usa a classe `abntex2` e o modelo **ufctex**, adaptado para os TCCs de Engenharia Elétrica da UFPI. É preciso ter uma distribuição LaTeX (TeX Live ou MiKTeX) com `abntex2`, `bibtex` e `makeglossaries`.

```bash
make
```

O `Makefile` executa `pdflatex`, `bibtex`, `makeglossaries`, `makeindex` e mais duas passadas de `pdflatex`, gerando `documento.pdf`. Também funciona em editores como VS Code (LaTeX Workshop) ou Overleaf.

## Como construir o .gitignore

Crie um arquivo chamado `.gitignore` (sem extensão) na raiz do repositório. Cada linha é um padrão de arquivo ou pasta que o Git deve ignorar. Para este projeto, os arquivos a ignorar são os gerados pela compilação e os relatórios de falha:

```gitignore
# Arquivos auxiliares do LaTeX
*.aux
*.bbl
*.blg
*.fdb_latexmk
*.fls
*.glg
*.glo
*.gls
*.idx
*.ilg
*.ind
*.ist
*.lof
*.log
*.lot
*.out
*.toc
*.synctex.gz

# Relatórios de falha da JVM (gerados pelo editor)
hs_err_pid*.log
replay_pid*.log

# Arquivos do sistema
Thumbs.db
.DS_Store
```

Dicas:

- **GitHub Desktop:** *Repository → Repository settings → Ignored Files*, cole o conteúdo e salve. Também é possível criar o arquivo manualmente na raiz da pasta.
- **Ao criar o repositório:** o GitHub Desktop e o site do GitHub têm o campo "Git ignore"; um modelo (por exemplo, `TeX`) cobre boa parte desses padrões.
- **O PDF final:** `documento.pdf` (cerca de 6 MB) pode ser mantido, para que a monografia fique disponível para leitura. Se preferir não versionar, acrescente `documento.pdf` ao `.gitignore`.
- **Arquivos já rastreados:** o `.gitignore` não afeta arquivos que o Git já acompanha. Se algum já foi commitado, remova-o do índice com `git rm --cached <arquivo>`.
- **Antes do primeiro commit:** confira em *Changes* se só aparecem os arquivos desejados.

## Licença e uso

O modelo ufctex é adaptado do ueceTeX (UECE) e do abnTeX2, distribuídos sob a LaTeX Project Public License. O conteúdo da monografia é de autoria de Händel Mateus Carvalho Sarmento; defina aqui a licença desejada para o texto e o código, se houver.
