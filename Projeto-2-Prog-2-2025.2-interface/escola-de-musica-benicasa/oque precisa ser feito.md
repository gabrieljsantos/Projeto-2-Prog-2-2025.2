8 Responsabilidades Detalhadas por M´odulo
Baseado nas propostas enviadas sobre cada m´odulo, a fim de organizar o projeto, esta se¸c˜ao descreve,
de forma detalhada, as responsabilidades esperadas de cada m´odulo do sistema. As descri¸c˜oes a seguir
devem ser interpretadas como diretrizes para o desenvolvimento, cabendo `as equipes projetar
suas solu¸c˜oes respeitando os limites e integra¸c˜oes aqui definidos.
M´odulo Login / Matr´ıcula
O m´odulo Login/Matr´ıcula ser´a respons´avel por centralizar toda a gest˜ao de identidade do sistema.
Espera-se que este m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Armazene os dados de identifica¸c˜ao de todos os usu´arios do sistema;
Realize a autentica¸c˜ao dos usu´arios no momento do acesso;
Identifique o perfil associado a cada usu´ario (Administrador, Professor ou Aluno);
Forne¸ca aos demais m´odulos mecanismos para valida¸c˜ao da existˆencia de usu´arios;
Garanta que apenas usu´arios v´alidos e ativos possam acessar o sistema.
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Realizar opera¸c˜oes acadˆemicas;
Manipular dados financeiros;
Controlar eventos, instrumentos ou produtos.
M´odulo ´ Area do Administrador
O m´odulo ´ Area do Administrador ser´a respons´avel por oferecer uma vis˜ao global e administrativa do
sistema. Espera-se que este m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Permita o cadastro de usu´arios, eventos, instrumentos e produtos da lanchonete;
Consolide informa¸c˜oes provenientes de todos os demais m´odulos;
Disponibilize relat´orios acadˆemicos, financeiros e patrimoniais;
Permita a visualiza¸c˜ao de pendˆencias de alunos e do funcionamento geral do sistema;
Execute rotinas de backup e restaura¸c˜ao dos dados.
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Executar tarefas acadˆemicas pr´oprias do professor;
Realizar compras ou inscri¸c˜oes como usu´ario comum;
Alterar dados operacionais de outros m´odulos diretamente.
6
M´odulo A¸c˜oes do Professor
Om´odulo A¸c˜oes do Professor ser´a respons´avel pelo gerenciamento das informa¸c˜oes acadˆemicas. Espera
se que este m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Permita o registro de notas e avalia¸c˜oes dos alunos;
Realize o c´alculo de m´edias e situa¸c˜oes acadˆemicas;
Disponibilize consultas sobre desempenho acadˆemico;
Organize informa¸c˜oes pedag´ogicas associadas aos cursos;
Forne¸ca dados acadˆemicos para consulta por outros m´odulos.
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Criar ou alterar cadastros de usu´arios;
Gerenciar dados financeiros;
Realizar inscri¸c˜oes em eventos ou empr´estimos de instrumentos.
M´odulo ´ Area do Aluno
Om´odulo ´ Area do Aluno ser´a respons´avel por centralizar as informa¸c˜oes dispon´ıveis ao aluno. Espera
se que este m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Permita a consulta de notas, m´edias e situa¸c˜ao acadˆemica;
Exiba informa¸c˜oes sobre eventos nos quais o aluno esteja inscrito;
Mostre os instrumentos atualmente emprestados ao aluno;
Apresente saldo e extrato financeiro;
Consolide informa¸c˜oes provenientes de diferentes m´odulos.
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Alterar dados acadˆemicos;
Criar eventos, produtos ou instrumentos;
Manipular diretamente dados de outros alunos.
M´odulo Empr´estimo e Devolu¸c˜ao de Instrumentos
Om´odulo de Empr´estimo e Devolu¸c˜ao de Instrumentos ser´a respons´avel pelo controle patrimonial dos
instrumentos musicais. Espera-se que este m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Gerencie o cadastro de instrumentos disponibilizados pela escola;
Registre empr´estimos e devolu¸c˜oes de instrumentos;
Identifique instrumentos dispon´ıveis e emprestados;
Aponte pendˆencias relacionadas a empr´estimos;
Disponibilize informa¸c˜oes para consulta por outros m´odulos.
7
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Criar ou validar usu´arios;
Controlar eventos ou dados financeiros;
Alterar informa¸c˜oes acadˆemicas.
M´odulo Eventos
O m´odulo Eventos ser´a respons´avel pela gest˜ao das atividades extracurriculares. Espera-se que este
m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Gerencie o cadastro de eventos previamente autorizados pelo Administrador;
Controle inscri¸c˜oes de alunos em eventos;
Gerencie limites de vagas e status dos eventos;
Disponibilize listas de eventos ativos e encerrados;
Forne¸ca informa¸c˜oes de inscri¸c˜ao para outros m´odulos.
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Criar usu´arios;
Gerenciar produtos ou estoque;
Alterar dados financeiros diretamente.
M´odulo Lanchonete / Cr´editos
Om´odulo Lanchonete/Cr´editos ser´a respons´avel pela gest˜ao financeira associada aos usu´arios. Espera
se que este m´odulo:
ˆ
ˆ
ˆ
ˆ
ˆ
Gerencie o cadastro de produtos da lanchonete;
Controle estoque e valores dos produtos;
Mantenha o saldo financeiro de alunos e professores;
Registre compras e movimenta¸c˜oes financeiras;
Disponibilize extratos e relat´orios financeiros.
Este m´odulo n˜ao dever´a:
ˆ
ˆ
ˆ
Criar ou alterar dados acadˆemicos;
Gerenciar eventos;
Cadastrar usu´arios.
8
9 Defini¸c˜ao de Pap´eis, Responsabilidades e Permiss˜oes
O sistema proposto dever´a ser estruturado com base em trˆes perfis principais de usu´arios: Admi
nistrador, Professor e Aluno. Cada perfil possuir´a permiss˜oes claramente delimitadas, garantindo
organiza¸c˜ao, seguran¸ca e integra¸c˜ao entre os m´odulos.
Perfis de Usu´ario
ˆ
ˆ
ˆ
Administrador: respons´avel por cadastrar, autorizar e supervisionar o funcionamento geral do
sistema.
Professor: respons´avel por atividades acadˆemicas e pedag´ogicas.
Aluno: usu´ario final que consulta informa¸c˜oes e realiza inscri¸c˜oes e solicita¸c˜oes.
M´odulo Login / Matr´ıcula
Este m´odulo ser´a respons´avel por gerenciar a identidade e o acesso dos usu´arios ao sistema.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Administrador realizar´a o cadastro inicial de alunos e professores;
O sistema armazenar´a identificador ´unico, nome, perfil e credenciais;
O m´odulo validar´a as credenciais no momento do login;
O perfil do usu´ario autenticado ser´a informado aos demais m´odulos.
Permiss˜oes:
ˆ
ˆ
Administrador: cadastrar, ativar, desativar usu´arios;
Professor e Aluno: apenas autenticar-se no sistema.
M´odulo ´ Area do Administrador
Este m´odulo ser´a respons´avel pela configura¸c˜ao e supervis˜ao global do sistema.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Administrador cadastrar´a cursos, eventos, instrumentos e produtos da lanchonete;
O Administrador autorizar´a a disponibiliza¸c˜ao de eventos e instrumentos;
O m´odulo consolidar´a informa¸c˜oes acadˆemicas, financeiras e patrimoniais;
O m´odulo fornecer´a relat´orios para tomada de decis˜ao.
Permiss˜oes:
ˆ
ˆ
Administrador: acesso total;
Professor e Aluno: sem acesso direto.
9
M´odulo A¸c˜oes do Professor
Este m´odulo ser´a respons´avel pelas atividades acadˆemicas conduzidas pelos docentes.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Professor registrar´a notas e avalia¸c˜oes;
O Professor consultar´a listas de alunos matriculados;
O m´odulo calcular´a m´edias e situa¸c˜oes acadˆemicas;
Os dados acadˆemicos ficar˜ao dispon´ıveis para consulta por outros m´odulos.
Permiss˜oes:
ˆ
ˆ
ˆ
Professor: cadastrar e alterar informa¸c˜oes acadˆemicas;
Administrador: apenas consulta;
Aluno: sem acesso direto.
M´odulo ´ Area do Aluno
Este m´odulo ser´a respons´avel pela intera¸c˜ao do aluno com o sistema.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Aluno consultar´a suas notas e m´edias;
O Aluno visualizar´a eventos dispon´ıveis;
O Aluno acompanhar´a empr´estimos de instrumentos;
O Aluno consultar´a saldo e extrato da lanchonete.
Permiss˜oes:
ˆ
ˆ
Aluno: acesso exclusivo `as pr´oprias informa¸c˜oes;
Professor e Administrador: sem acesso.
M´odulo Empr´estimo e Devolu¸c˜ao de Instrumentos
Este m´odulo controlar´a o patrimˆonio da escola relacionado aos instrumentos musicais.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Administrador cadastrar´a os instrumentos;
O Aluno solicitar´a o empr´estimo de instrumentos dispon´ıveis;
O m´odulo registrar´a empr´estimos e devolu¸c˜oes;
Pendˆencias de devolu¸c˜ao ser˜ao informadas ao Administrador.
Permiss˜oes:
ˆ
ˆ
ˆ
Administrador: cadastro e libera¸c˜ao de instrumentos;
Aluno: solicita¸c˜ao e consulta;
Professor: apenas consulta.
10
M´odulo Eventos
Este m´odulo ser´a respons´avel pela gest˜ao de eventos acadˆemicos e culturais.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Administrador cadastrar´a eventos;
O Administrador autorizar´a sua divulga¸c˜ao;
O Aluno realizar´a inscri¸c˜oes em eventos;
O m´odulo controlar´a vagas e status das inscri¸c˜oes.
Permiss˜oes:
ˆ
ˆ
ˆ
Administrador: cadastro e autoriza¸c˜ao;
Aluno: inscri¸c˜ao e consulta;
Professor: apenas consulta.
M´odulo Lanchonete / Cr´editos
Este m´odulo ser´a respons´avel pelo controle financeiro da lanchonete.
Responsabilidades:
ˆ
ˆ
ˆ
ˆ
O Administrador cadastrar´a produtos e pre¸cos;
O Administrador adicionar´a cr´editos `as contas dos usu´arios;
O Aluno realizar´a compras;
O m´odulo atualizar´a automaticamente o estoque e o saldo.
Permiss˜oes:
ˆ
ˆ
ˆ
Administrador: cadastro e cr´edito;
Aluno: compra e consulta;
Professor: apenas consulta de saldo.