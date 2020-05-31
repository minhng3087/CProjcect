#include "gtk/gtk.h"
#include <string.h>
#include <stdlib.h>
#include "inc/btree.h"
#include <ctype.h>

/****************************/
void soundex(char *chAlphaName, char *strResult)
{
     int i;
     int j = 0;
     char SCode = '0';
     char PrevCode = '0';
     char CharTemp = '0';

     for (i = 0; i < strlen(chAlphaName); i++)
     {
          chAlphaName[i]=tolower(chAlphaName[i]);
     }

     for (i = 0; (i < strlen(chAlphaName) && j < 4); i++)
     {
          CharTemp = chAlphaName[i];

          switch(CharTemp)
          {
               case 'r':
                    SCode = '6';
                    break;
               case 'm':
               case 'n':
                    SCode='5';
                    break;
               case 'l':
                    SCode='4';
                    break;
               case 'd':
               case 't':
                    SCode='3';
                    break;
               case 'c':
               case 'g':
               case 'j':
               case 'k':
               case 'q':
               case 's':
               case 'x':
               case 'z':
                    SCode = '2';
                    break;
               case 'b':
               case 'f':
               case 'p':
               case 'v':
                    SCode = '1';
                    break;
               default:
                    SCode = '0';
                    break;
          }

          if (SCode > '0' || j==0)
          {
               //SCode la chu cai dau tien
               if (j == 0)
               {
                    strResult[j] = chAlphaName[j];
                   // strResult[j+1] = '\0';

                    j++;
               }
               else if (SCode != PrevCode)
                    {
                         strResult[j] = SCode;
                    //     strResult[j+1] = '\0';

                         j++;
                    }
          }


          if (CharTemp == 'h' || CharTemp == 'w')
          {
               SCode = PrevCode;
          }

          PrevCode = SCode;
          SCode = '0';

     }

     for (i = j; i < 4; i++)
     {
          strResult[i] = '0' ;
     }

     strResult[i]='\0';
}



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


GtkWidget *dialog_add;
GtkWidget *button_add;
GtkWidget *button_add_ok;
GtkWidget *button_cancel_add;
GtkWidget *entry_add_word;
GtkTextView *entry_add_mean;


BTA *evdata;
BTA *sound;

void search_word(GObject *object,gpointer user_data) {
  const gchar *word;
  char data[100000];
  char sou[100];
  char mean[100000];
  int i;
  GtkTextBuffer *buff;
  word = gtk_entry_get_text(GTK_ENTRY(entry_find));
  if(strcmp(word,"")==0){ 
    buff = gtk_text_view_get_buffer(textview_result); 
    gtk_text_buffer_set_text(buff,"\n     ===> ERROR: No word is entered! <===",strlen("\n     ===> ERROR: No word is entered! <==="));
    return;
  }
  if(bfndky(evdata,(char *)word,&i) == 0) {
    btsel(evdata,(char*)word,data,100000,&i);
    strcpy(mean, data);
    buff = gtk_text_view_get_buffer(textview_result);
    gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));
    return;
  }
  // else {
  //   sound = btopn("../data/soundex.dat",0,0);
  //   soundex((char*)word,sou);
  //   if(bfndky(sound,(char*)sou,&i)==0){
  //     btsel(sound,sou,mean,100000,&i);
  //     buff = gtk_text_view_get_buffer(textview_result); //Returns the GtkTextBuffer being displayed by this text view
  //     gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));
  //    }  
  // }
  
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
  g_object_unref(completion);

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
  gtk_entry_set_text(GTK_ENTRY(entry_find),""); //tao ra mot widget 
  buff = gtk_text_view_get_buffer(textview_result);
  gtk_text_buffer_set_text(buff,"",1);
  gtk_dialog_run(GTK_DIALOG(dialog_delete));
  gtk_widget_hide(GTK_WIDGET(dialog_delete)); //khi chay xong phai an
}

void button_detele_clicked(GObject *object,gpointer user_data){
  const gchar *word;
  GtkWidget *message;
  int i;
  word = gtk_entry_get_text(GTK_ENTRY(entry_delete));//lay tu o entry_delete
  printf("%s\n",word);
  if(bfndky(evdata,(char*)word,&i)==0){ // kiem tra cai data
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Xoa thanh cong!","Info");
    btdel(evdata,(char*)word);
    gtk_window_set_title(GTK_WINDOW(message),"Infor");
    gtk_dialog_run(GTK_DIALOG(message));
    gtk_widget_destroy(message);
    printf("Xoa thanh cong !!!\n");
  }
  else{//khi k co tu
    message = gtk_message_dialog_new(GTK_WINDOW(dialog_delete),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"Khong ton tai tu nay!","Info");
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
     const gchar *word;
     const gchar *mean;
     int i;
     g_assert(GTK_IS_ENTRY(entry_add_word));
     word = gtk_entry_get_text(GTK_ENTRY(entry_add_word));
     buff = gtk_text_view_get_buffer(entry_add_mean);
     gtk_text_buffer_get_start_iter(buff, &first);//
     gtk_text_buffer_get_end_iter(buff, &last);
     mean = gtk_text_buffer_get_text(buff, &first, &last, FALSE);
     if(strcmp(word, "") == 0) return;
     else
          if(bfndky(evdata, (char*)word, &i) == 0){
               message = gtk_message_dialog_new(GTK_WINDOW(dialog_add),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_ERROR,GTK_BUTTONS_OK,"This word  existed!","Info");
               gtk_window_set_title(GTK_WINDOW(message), "Information");
               gtk_dialog_run(GTK_DIALOG(message));
               gtk_widget_destroy(message);
          }
          else{
               sound = btopn("../data/soundex.dat",0,0);
               soundex(sound, (char*)word);
               btcls(sound);
               message = gtk_message_dialog_new(GTK_WINDOW(dialog_add),GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Completed !","Info");
               gtk_window_set_title(GTK_WINDOW(message), "Information");
               gtk_dialog_run(GTK_DIALOG(message));
               gtk_widget_destroy(message);
          }
     gtk_entry_set_text(GTK_ENTRY(entry_add_word),"");
     buff = gtk_text_view_get_buffer(entry_add_mean);
     gtk_text_buffer_set_text(buff, "", 1);
     list_word_search(entry_find);
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





/****************************************************************/
int main (int argc, char *argv[]) {
     evdata = btopn("../data/english-vietnamese.dat",0,0);
     GtkBuilder  *builder; 
     gtk_init (&argc, &argv);
     builder = gtk_builder_new ();
     gtk_builder_add_from_file (builder, "/home/minh/myProject/cmake-gtk/dic.glade", NULL);
     //////////////////////////////////////////////


     window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
     find_button = GTK_WIDGET(gtk_builder_get_object (builder, "button_find"));
     entry_find = GTK_WIDGET(gtk_builder_get_object(builder,"entry_find"));
     quit = GTK_WIDGET(gtk_builder_get_object(builder,"button_quit"));
     textview_result = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"textview"));
     dialog_delete = GTK_DIALOG(gtk_builder_get_object(builder, "dialog_delete"));
     delete_button = GTK_WIDGET(gtk_builder_get_object(builder, "button_remove"));
     button_delete = GTK_WIDGET(gtk_builder_get_object(builder, "button_delete"));
     button_cancel_delete = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancel_delete"));
     entry_delete = GTK_WIDGET(gtk_builder_get_object(builder, "entry_delete"));
     dialog_add = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_add"));

     button_add_ok = GTK_WIDGET(gtk_builder_get_object(builder, "button_add_ok"));
     button_add = GTK_WIDGET(gtk_builder_get_object(builder, "button_add"));
     button_cancel_add = GTK_WIDGET(gtk_builder_get_object(builder, "button_cancel_add"));
     entry_add_word = GTK_WIDGET(gtk_builder_get_object(builder, "entry_add_word"));
     entry_add_mean = GTK_TEXT_VIEW(gtk_builder_get_object(builder, "entry_add_mean"));
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

     //thoat chuong trinh
     g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);


     g_object_unref(G_OBJECT(builder));
     list_word_search(entry_find);
     gtk_widget_show(window);
     gtk_main();

}