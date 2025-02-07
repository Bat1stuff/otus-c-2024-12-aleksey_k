#include <gtk/gtk.h>
#include <glib.h>

enum {
    COLUMN_NAME,
    NUM_COLUMNS
};

void buildTreeModel(const gchar *dirPath, GtkTreeStore *tree_store, GtkTreeIter *current, GtkTreeIter *parent)
{
    const gchar *filename;
    // Открываем директорию
    GDir *dir = g_dir_open(dirPath, 0, NULL);
    if (!dir) {
        g_printerr("Ошибка открытия директории: %s\n", dirPath);
        return;
    }

    while ((filename = g_dir_read_name(dir)) != NULL) {
        GtkTreeIter child;
        gchar *full_path = g_build_filename(dirPath, filename, NULL);

        // Добавляем элемент в модель
        gtk_tree_store_append(tree_store, current, parent);
        gtk_tree_store_set(tree_store, current, COLUMN_NAME, g_strdup(filename), -1);

        // Если это директория рекурсивно проходим по ней
        if (g_file_test(full_path, G_FILE_TEST_IS_DIR)) {
            buildTreeModel(full_path, tree_store, &child, current);
        }

        g_free(full_path);
    }

    g_dir_close(dir);
}

void renderTreeViewWidget(GtkWidget *window)
{
    // Получаем текущую директорию
    const gchar *currentDir = g_get_current_dir();
    // Создаем модель данных (GtkTreeStore)
    GtkTreeStore *tree_store = gtk_tree_store_new(NUM_COLUMNS, G_TYPE_STRING, G_TYPE_INT);
    GtkTreeIter current;
    // Строим дерево
    buildTreeModel(currentDir, tree_store, &current, NULL);
    // Создаем виджет GtkTreeView и подключаем модель
    GtkWidget *tree_view = gtk_tree_view_new_with_model(GTK_TREE_MODEL(tree_store));
    // Колонка "File system"
    GtkCellRenderer *renderer = gtk_cell_renderer_text_new();
    GtkTreeViewColumn *column = gtk_tree_view_column_new_with_attributes("File system", renderer, "text", COLUMN_NAME, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    // Добавляем TreeView в окно
    gtk_container_add(GTK_CONTAINER(window), tree_view);
}

static void activate(GtkApplication* app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "TreeView");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 400);

    renderTreeViewWidget(window);

    gtk_widget_show_all(window);
}

int main(int argc, char **argv)
{
    GtkApplication *app = gtk_application_new("org.example.TreeModelExample", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    const int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
