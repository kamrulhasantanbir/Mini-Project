#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GtkWidget *window;
GtkWidget *fixed;
GtkWidget *label1, *label2, *label3, *label4;
GtkWidget *button,*button1;
GtkWidget *entry1, *entry2;
GtkWidget *combo;

int department;
const char *department_name;
double ssc, hsc;
const char *ssc_text;
const char *hsc_text;
int waiver_percent;

void calculate_waiver() {
    double min_result;
    if(ssc > hsc) {
        min_result = hsc;
    } else {
        min_result = ssc;
    }

    if (strcmp(department_name, "LLB") == 0 || strcmp(department_name, "B_pharm") == 0 || min_result < 3.5) {
        waiver_percent = 0;
    } else if (min_result > 5.0) {
        waiver_percent = 70;
    } else if (min_result == 5.0) {
        waiver_percent = 40;
    } else if (min_result >= 4.8) {
        waiver_percent = 30;
    } else if (min_result >= 4.5) {
        waiver_percent = 20;
    } else if (min_result >= 4.0) {
        waiver_percent = 15;
    } else if (min_result >= 3.5) {
        waiver_percent = 10;
    } else {
        waiver_percent = 0;
    }

    char waiver[50], semester[50], credit_hours[50], semester_fee[50], credit_fee[50], admission_fee[50], other_fee[50],total_cost_str[50],result_text [1000];
    sprintf(waiver, "%d", waiver_percent);
       
    FILE *file = fopen("list.txt", "r");

    int start_line = department * 7+1;
    char buffer[100];
    int line_counter = 0;

    while (fgets(buffer, sizeof(buffer), file)) {
        if (line_counter == start_line) {
            strcpy(semester, buffer);
        } else if (line_counter == start_line + 1) {
            strcpy(credit_hours, buffer);
        } else if (line_counter == start_line + 2) {
            strcpy(semester_fee, buffer);
        } else if (line_counter == start_line + 3) {
            strcpy(credit_fee, buffer);
        } else if (line_counter == start_line + 4) {
            strcpy(admission_fee, buffer);
        } else if (line_counter == start_line + 5) {
            strcpy(other_fee, buffer);
        } else if (line_counter >= start_line + 6) {
            break;
        }
        line_counter++;
    }
    fclose(file);

    double adjusted_credit_fee = atof(credit_fee) - atof(credit_fee) * (waiver_percent / 100.0);
    sprintf(credit_fee, "%.2f", adjusted_credit_fee);

    double total_cost = atof(admission_fee) + atof(other_fee) + atof(semester_fee) * atof(semester) + atof(credit_hours) * adjusted_credit_fee;

    sprintf(total_cost_str, "%.2f", total_cost);

    sprintf(result_text, "Waiver: %s%%\nSemester: %sSemester Fee: %sCredit Hours: %sCredit Fee: %s\nAdmission Fee: %sOther Fee: %s\nTotal Cost: %sTk", waiver, semester, semester_fee, credit_hours, credit_fee, admission_fee, other_fee, total_cost_str);

    gtk_label_set_text(GTK_LABEL(label4), result_text);

}

void on_submit(GtkWidget *widget, gpointer user_data) {
    department_name = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo));
    department = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));
    ssc_text = gtk_entry_get_text(GTK_ENTRY(entry1));
    ssc = atof(ssc_text);
    hsc_text = gtk_entry_get_text(GTK_ENTRY(entry2));
    hsc = atof(hsc_text);

    calculate_waiver();
}

void on_about(GtkWidget *widget, gpointer user_data) {

    GtkWidget * new_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(new_window), "About us");
    gtk_window_set_default_size(GTK_WINDOW(new_window), 400, 700);
    gtk_window_set_resizable(GTK_WINDOW(new_window), FALSE);
    g_signal_connect(new_window, "destroy", G_CALLBACK(gtk_widget_destroy), NULL);
    
    FILE *file;
    file=fopen("about.txt","r");
    char about[2000];
    char buffer[100];
    fgets(buffer,100,file);
    strcpy(about,buffer);
    while(fgets(buffer,100,file)){
    strcat(about,buffer);
    }
    
    GtkWidget *label_about=gtk_label_new(about);
    gtk_container_add(GTK_CONTAINER(new_window),label_about);
    gtk_widget_show_all(new_window);

}

int main() {
    gtk_init(NULL, NULL);

    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "NUB Admission Costing Calculator");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 700);
    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(window), fixed);

    label1 = gtk_label_new("Select your department:");
    label2 = gtk_label_new("Enter your SSC result:");
    label3 = gtk_label_new("Enter your HSC result:");
    label4 = gtk_label_new(NULL);

    button = gtk_button_new_with_label("Submit");
    button1= gtk_button_new_with_label("About us");

    entry1 = gtk_entry_new();
    entry2 = gtk_entry_new();

    combo = gtk_combo_box_text_new();
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "BBA");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "LLB");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "EEE");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "TEXTILE");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "CSE");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "CIVIL");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "ME");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "ECE");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "B_pharm");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "ELL");
    gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "BANGLA");

    gtk_fixed_put(GTK_FIXED(fixed), label1, 100, 50);
    gtk_fixed_put(GTK_FIXED(fixed), label2, 100, 100);
    gtk_fixed_put(GTK_FIXED(fixed), label3, 100, 150);
    gtk_fixed_put(GTK_FIXED(fixed), combo, 300, 50);
    gtk_fixed_put(GTK_FIXED(fixed), entry1, 300, 100);
    gtk_fixed_put(GTK_FIXED(fixed), entry2, 300, 150);
    gtk_fixed_put(GTK_FIXED(fixed), button, 250, 200);
    gtk_fixed_put(GTK_FIXED(fixed), label4, 100, 250);
    gtk_fixed_put(GTK_FIXED(fixed), button1, 250, 650);

    g_signal_connect(button, "clicked", G_CALLBACK(on_submit), NULL);
    g_signal_connect(button1, "clicked", G_CALLBACK(on_about), NULL);

    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
