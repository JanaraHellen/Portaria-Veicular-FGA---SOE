#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <telebot.h>
#include <sqlite3.h>

#define SIZE_OF_ARRAY(array) (sizeof(array) / sizeof(array[0]))

// Declarando handle e ret aqui
telebot_handler_t handle;
telebot_error_e ret;

// Função para executar comandos SQL no SQLite
int execute_sql_command(sqlite3 *db, const char *sql_command) {
    char *errMsg = 0;
    int result = sqlite3_exec(db, sql_command, 0, 0, &errMsg);
    if (result != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", errMsg);
        sqlite3_free(errMsg);
        return -1;
    }
    return 0;
}

// Função para listar veículos cadastrados
void list_vehicles(sqlite3 *db, telebot_handler_t handle, int chat_id) {
    const char *select_sql = "SELECT name, plate FROM vehicles;";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, select_sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Falha ao preparar a instrução SELECT: %s\n", sqlite3_errmsg(db));
        return;
    }

    printf("Listando veículos:\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *name = sqlite3_column_text(stmt, 0);
        const unsigned char *plate = sqlite3_column_text(stmt, 1);

        char str[1024];
        snprintf(str, sizeof(str), "Nome: %s\nPlaca: %s", name, plate);

        telebot_send_message(handle, chat_id, str, "HTML", false, false, 0, "");
    }

    sqlite3_finalize(stmt);
}

int main(int argc, char *argv[]) {
    printf("Bem-vindo ao Echobot\n");

    FILE *fp = fopen(".token", "r");
    if (fp == NULL) {
        printf("Falha ao abrir o arquivo .token\n");
        return -1;
    }

    char token[1024];
    if (fscanf(fp, "%s", token) == 0) {
        printf("Falha ao ler o token\n");
        fclose(fp);
        return -1;
    }
    printf("Token: %s\n", token);
    fclose(fp);

    // Inicializa o Telebot
    if (telebot_create(&handle, token) != TELEBOT_ERROR_NONE) {
        printf("Falha ao criar o Telebot\n");
        return -1;
    }

    telebot_user_t me;
    if (telebot_get_me(handle, &me) != TELEBOT_ERROR_NONE) {
        printf("Falha ao obter informações do bot\n");
        telebot_destroy(handle);
        return -1;
    }

    printf("ID: %d\n", me.id);
    printf("Primeiro Nome: %s\n", me.first_name);
    printf("Nome de Usuário: %s\n", me.username);

    telebot_put_me(&me);

    int count, offset = -1;
    telebot_error_e ret;
    telebot_update_type_e update_types[] = {TELEBOT_UPDATE_TYPE_MESSAGE};

    // Conexão com o banco de dados SQLite
    sqlite3 *db;
    if (sqlite3_open("seu_arquivo_de_banco_de_dados.db", &db) != SQLITE_OK) {
        fprintf(stderr, "Falha ao abrir o banco de dados\n");
        telebot_destroy(handle);
        return -1;
    }

    while (1) {
        telebot_update_t *updates;
        ret = telebot_get_updates(handle, offset, 20, 0, update_types, 0, &updates, &count);
        if (ret != TELEBOT_ERROR_NONE)
            continue;

        printf("Número de atualizações: %d\n", count);
        for (int index = 0; index < count; index++) {
            telebot_message_t message = updates[index].message;

            if (message.text) {
                printf("%s: %s \n", message.from->first_name, message.text);

                // Verifica se a mensagem contém "/cadastrar"
                if (strstr(message.text, "/cadastrar") != NULL) {
                    // Extrair o nome e a placa da mensagem
                    const char *command = "/cadastrar";
                    size_t command_len = strlen(command);
                    const char *data_start = message.text + command_len;

                    // Encontrar o início do nome e da placa
                    while (*data_start == ' ' && *data_start != '\0') {
                        data_start++;
                    }

                    // Verificar se há dados para cadastrar
                    if (*data_start != '\0') {
                        char name[256], plate[256];
                        if (sscanf(data_start, "%s %s", name, plate) == 2) {
                            // Lógica para cadastrar veículo com o nome e a placa fornecidos
                            char insert_sql[1024];
                            snprintf(insert_sql, sizeof(insert_sql), "INSERT INTO vehicles (name, plate) VALUES ('%s', '%s');", name, plate);
                            if (execute_sql_command(db, insert_sql) != 0) {
                                printf("Falha ao inserir dados no banco de dados\n");
                            } else {
                                printf("Dados do veículo inseridos com sucesso\n");
                            }

                            // Informar ao usuário que o veículo foi cadastrado
                            char str[256];
                            snprintf(str, sizeof(str), "Veículo cadastrado:\nNome: %.100s\nPlaca: %.100s", name, plate);
                            ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");
                            if (ret != TELEBOT_ERROR_NONE) {
                                printf("Falha ao enviar a mensagem: %d \n", ret);
                            }
                        } else {
                            // Informar ao usuário que os dados fornecidos não estão corretos
                            char str[256];
                            snprintf(str, sizeof(str), "Por favor, forneça dados no formato correto: /cadastrar Nome Placa");
                            ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");
                            if (ret != TELEBOT_ERROR_NONE) {
                                printf("Falha ao enviar a mensagem: %d \n", ret);
                            }
                        }
                    } else {
                        // Informar ao usuário que nenhum dado foi fornecido
                        char str[256];
                        snprintf(str, sizeof(str), "Por favor, forneça dados para cadastrar o veículo no formato: /cadastrar Nome Placa");
                        ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");
                        if (ret != TELEBOT_ERROR_NONE) {
                            printf("Falha ao enviar a mensagem: %d \n", ret);
                        }
                    }
                } else if (strstr(message.text, "/listar") != NULL) {
                    // Listar veículos cadastrados
                    list_vehicles(db, handle, message.chat->id);
                } else if (strstr(message.text, "/dice") != NULL) {
                    telebot_send_dice(handle, message.chat->id, false, 0, "");
                } else {
                    // Comportamento padrão para outras mensagens
                    char str[4096];
                    if (strstr(message.text, "/start") != NULL) {
                        snprintf(str, SIZE_OF_ARRAY(str), "Olá %s", message.from->first_name);
                    } else {
                        snprintf(str, SIZE_OF_ARRAY(str), "<i>%s</i>", message.text);
                    }
                    ret = telebot_send_message(handle, message.chat->id, str, "HTML", false, false, updates[index].message.message_id, "");
                    if (ret != TELEBOT_ERROR_NONE) {
                        printf("Falha ao enviar a mensagem: %d \n", ret);
                    }
                }
            }
            offset = updates[index].update_id + 1;
        }
        telebot_put_updates(updates, count);

        sleep(1);
    }

    // Fecha a conexão com o banco de dados SQLite
    sqlite3_close(db);

    // Destrói o handle do Telebot
    telebot_destroy(handle);

    return 0;
}
