#include "gtk/gtk.h"
#include <string.h>
#include <stdlib.h>
#include "inc/btree.h"
#include <ctype.h>


/****************************/



/***********************/

GtkWidget *window;
GtkWidget *entry_find;
GtkTextView *textview_result;
GtkWidget *quit;
GtkWidget *find_button;
GtkEntryCompletion *completion;
GtkDialog *dialog_delete;
GtkWidget *button_delete;
GtkWidget *button_cancel_delete;
GtkWidget *entry_delete;
GtkWidget *delete_button;
GtkWidget *box_find;

GtkDialog *dialog_add;
GtkWidget *button_add;
GtkWidget *button_add_ok;
GtkWidget *button_cancel_add;
GtkWidget *entry_add_word;
GtkTextView *entry_add_mean;

GtkDialog *dialog_edit;
GtkWidget *button_edit;
GtkWidget *button_edit_ok;
GtkWidget *button_cancel_edit;
GtkWidget *entry_edit_word;
GtkTextView *entry_edit_mean;


BTA *evdata;

void search_word(GObject *object,gpointer user_data) {
  const gchar *word;
  char data[100000];
  char mean[100000];
  int i;
  GtkTextBuffer *buff;
  word = gtk_entry_get_text(GTK_ENTRY(entry_find));
  if(strcmp(word,"")==0){ 
    buff = gtk_text_view_get_buffer(textview_result); 
    gtk_text_buffer_set_text(buff,"\nBạn chưa nhập từ cần tìm",strlen("\nBạn chưa nhập từ cần tìm"));
    return;
  }
  if(bfndky(evdata,(char *)word,&i) == 0) {
    btsel(evdata,(char*)word,data,100000,&i);
    strcpy(mean, data);
    buff = gtk_text_view_get_buffer(textview_result);
    gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));
    return;
  }
  else {
    buff = gtk_text_view_get_buffer(textview_result); 
    gtk_text_buffer_set_text(buff,"\nTừ này hiện chưa có trong từ điển",strlen("\nTừ này hiện chưa có trong từ điển"));
    return;
  }
  
}



void list_word_search(GtkWidget* text)
{
  GtkListStore *liststore; 
  GtkTreeIter iter;

  char word[1000];
  char mean[1000];
  int rsize;
  completion = gtk_entry_completion_new();
  gtk_entry_completion_set_text_column(completion,0);
  gtk_entry_set_completion(GTK_ENTRY(text),completion);

  liststore = gtk_list_store_new(1,G_TYPE_STRING);
  btpos(evdata,1);
  while(btseln(evdata,word,mean,1000,&rsize)==0){
    gtk_list_store_append(liststore,&iter);
    gtk_list_store_set(liststore,&iter,0,word,-1);
  }
  gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));
  g_object_unref(GTK_TREE_MODEL(liststore));
}


/***************************************************/
void clicked_delete(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  gtk_entry_set_text(GTK_ENTRY(entry_find),"");
  buff = gtk_text_view_get_buffer(textview_result);
  gtk_text_buffer_set_text(buff,"",1);
  gtk_dialog_run(GTK_DIALOG(dialog_delete));
  gtk_widget_hide(GTK_WIDGET(dialog_delete));
}

void button_detele_clicked(GObject *object,gpointer user_data){
  const gchar *word;
  GtkWidget *message;
  int i;
  word = gtk_entry_get_text(GTK_ENTRY(entry_delete));
  if(bfndky(evdata,(char*)word,&i)==0){ 
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Xóa thành công!","Info");
    btdel(evdata,(char*)word);
    gtk_window_set_title(GTK_WINDOW(message),"Infor");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  else{
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Không tồn tại từ này!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Infor");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  gtk_entry_set_text(GTK_ENTRY(entry_delete),"");
  list_word_search(entry_find);
}

void button_cancel_delete_clicked(GObject *object,gpointer user_data){
  gtk_widget_hide(GTK_WIDGET(dialog_delete));
}


/****************************************************/
void button_add_ok_clicked(GObject *object,gpointer user_data){
     GtkTextBuffer *buff;
     GtkWidget *message;
     GtkTextIter first,last;
     gchar *word;
     gchar *mean;
     int i;
     word = gtk_entry_get_text(GTK_ENTRY(entry_add_word));
     buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry_add_mean));
     gtk_text_buffer_get_bounds (buff, &first, &last);
     mean = gtk_text_buffer_get_text(buff, &first, &last, FALSE);
     if(strcmp(word, "") == 0) return;
     else
          if(bfndky(evdata,word, &i) == 0){
               message = gtk_message_dialog_new(GTK_WINDOW(dialog_add),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Từ này đã tồn tại!","Info");
               gtk_window_set_title(GTK_WINDOW(message), "Infor");
               gtk_dialog_run(GTK_DIALOG(message));
               gtk_widget_destroy(message);
          }
          else {
          btins(evdata,word,mean,strlen(mean)+1);
          message = gtk_message_dialog_new(GTK_WINDOW(dialog_add),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Thêm thành công !","Info");
               gtk_window_set_title(GTK_WINDOW(message), "Infor");
               gtk_dialog_run(GTK_DIALOG(message));
               gtk_widget_destroy(message);
          }
     gtk_entry_set_text(GTK_ENTRY(entry_add_word),"");
     buff = gtk_text_view_get_buffer(entry_add_mean);
     gtk_text_buffer_set_text(buff, "", 1);
     list_word_search(entry_find);
     list_word_search(entry_delete);
}    
void clicked_add(GObject *object, gpointer user_data){
    GtkTextBuffer *buff;
    gtk_entry_set_text(GTK_ENTRY(entry_find), "");
    buff = gtk_text_view_get_buffer(textview_result);
    gtk_text_buffer_set_text(buff,"",1);
    gtk_dialog_run(GTK_DIALOG(dialog_add));
    gtk_widget_hide(GTK_WIDGET(dialog_add));
}

void button_cancel_add_clicked(GObject *object, gpointer user_data){
     gtk_widget_hide(GTK_WIDGET(dialog_add));
}

/****************************************************************/
void clicked_edit(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  gtk_entry_set_text(GTK_ENTRY(entry_find),"");
  buff = gtk_text_view_get_buffer(textview_result);
  gtk_text_buffer_set_text(buff,"",1);
  gtk_dialog_run(GTK_DIALOG(dialog_edit));
  gtk_widget_hide(GTK_WIDGET(dialog_edit));
}
void button_edit_ok_clicked(GObject *object,gpointer user_data){
  GtkTextBuffer *buff;
  GtkWidget *message;
  GtkTextIter first,last;
  gchar *word;
  gchar *mean;
  int i;
  word = gtk_entry_get_text(GTK_ENTRY(entry_edit_word));
  buff = gtk_text_view_get_buffer(GTK_TEXT_VIEW(entry_edit_mean));
  gtk_text_buffer_get_bounds (buff, &first, &last);
  mean = gtk_text_buffer_get_text(buff, &first, &last, FALSE);
  if(strcmp(word, "") == 0) return;
  if(bfndky(evdata,(char*)word,&i)==0){
    btupd(evdata,word,mean,strlen(mean)+1);
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_edit),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Sửa thành công!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  else{
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_edit),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Từ này đã tồn tại!","Info");
    gtk_window_set_title(GTK_WINDOW(message),"Information");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
  }
  gtk_entry_set_text(GTK_ENTRY(entry_edit_word),"");
  buff = gtk_text_view_get_buffer(entry_edit_mean);
  gtk_text_buffer_set_text(buff,"",1);
  list_word_search(entry_find);
}
void button_cancel_edit_clicked(GObject *object,gpointer user_data){
   gtk_widget_hide(GTK_WIDGET(dialog_edit));
}




/****************************************************************/
int main (int argc, char *argv[]) {
    evdata = btopn("../data/words.dat",0,0);
    GtkBuilder  *builder; 
    GError *error = NULL;
    gtk_init (&argc, &argv);
    builder = gtk_builder_new ();
    gtk_builder_add_from_file (builder, "../dic.glade", &error);
    //////////////////////////////////////////////


    window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
    find_button = GTK_WIDGET(gtk_builder_get_object (builder, "button_find"));
    entry_find = GTK_WIDGET(gtk_builder_get_object(builder,"entry_find"));
    quit = GTK_WIDGET(gtk_builder_get_object(builder,"button_quit"));
    textview_result = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"textview"));

    button_delete = GTK_WIDGET(gtk_builder_get_object(builder, "button_delete"));
    dialog_delete = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_delete"));
    delete_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_remove"));
    button_cancel_delete = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancel_delete"));
    entry_delete = GTK_WIDGET(gtk_builder_get_object(builder, "entry_delete"));

    button_add = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
    dialog_add = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_add"));
    button_add_ok = GTK_WIDGET(gtk_builder_get_object(builder, "button_add_ok"));
    button_cancel_add = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancel_add"));
    entry_add_word = GTK_WIDGET(gtk_builder_get_object(builder, "entry_add_word"));
    entry_add_mean = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "entry_add_mean"));


    button_edit = GTK_WIDGET(gtk_builder_get_object(builder, "button_edit"));
    dialog_edit = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_edit"));
    button_edit_ok = GTK_WIDGET(gtk_builder_get_object(builder, "button_edit_ok"));
    button_cancel_edit = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancel_edit"));
    entry_edit_word = GTK_WIDGET(gtk_builder_get_object(builder, "entry_edit_word"));
    entry_edit_mean = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "entry_edit_mean"));
    quit = GTK_WIDGET(gtk_builder_get_object(builder,"button_quit"));
    gtk_builder_connect_signals(builder,NULL);

    //////////////////////////////////
    g_signal_connect(G_OBJECT(find_button), "clicked", G_CALLBACK (search_word), NULL);
    g_signal_connect(G_OBJECT(delete_button), "clicked", G_CALLBACK(clicked_delete), NULL);
    g_signal_connect(G_OBJECT(button_delete), "clicked", G_CALLBACK(button_detele_clicked),NULL);
    g_signal_connect(G_OBJECT(button_cancel_delete), "clicked", G_CALLBACK(button_cancel_delete_clicked), NULL);
    list_word_search(entry_delete);

    g_signal_connect(G_OBJECT(button_add), "clicked", G_CALLBACK(clicked_add), NULL);
    g_signal_connect(G_OBJECT(button_add_ok), "clicked", G_CALLBACK(button_add_ok_clicked), NULL);
    g_signal_connect(G_OBJECT(button_cancel_add), "clicked", G_CALLBACK(button_cancel_add_clicked), NULL);

    g_signal_connect(G_OBJECT(button_edit), "clicked", G_CALLBACK(clicked_edit), NULL);
    g_signal_connect(G_OBJECT(button_edit_ok), "clicked", G_CALLBACK(button_edit_ok_clicked), NULL);
    g_signal_connect(G_OBJECT(button_cancel_edit), "clicked", G_CALLBACK(button_cancel_edit_clicked), NULL);

    //thoat chuong trinh
    g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);


    g_object_unref(G_OBJECT(builder));
    list_word_search(entry_find);
    gtk_widget_show(window);
    gtk_main();
    btcls(evdata);

}