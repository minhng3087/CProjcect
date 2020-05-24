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

enum{
  COL_WORD = 0,
  COL_NUM
};
GtkWidget *window;
GtkTextView *textview_search;
GtkTextView *textview_result;
GtkWidget *quit;
GtkWidget *find_button;
GtkEntryCompletion *completion;


BTA *evdata;
BTA *sound;




void on_button_find_ev_clicked(GObject *object,gpointer user_data) {
  const gchar *word;
  char data[100000];
  char sou[100];
  char mean[100000];
  int i,j=0;
  GtkTextBuffer *buff;
  word = gtk_entry_get_text(GTK_ENTRY(textview_search));
  if(strcmp(word,"")==0){ //kiem tra xem nhap chua
    buff = gtk_text_view_get_buffer(textview_result); // Returns the GtkTextBuffer being displayed by this text view.
    gtk_text_buffer_set_text(buff,"\n     ===> ERROR: No word is entered! <===",strlen("\n     ===> ERROR: No word is entered! <==="));
    return;
  }
  if(bfndky(evdata,(char *)word,&i) == 0) {
    btsel(evdata,(char*)word,data,1000,&i);
    strcpy(mean, data);
    buff = gtk_text_view_get_buffer(textview_result); //Returns the GtkTextBuffer being displayed by this text view
    gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));
  }
  else {
    sound = btopn("../data/soundex.dat",0,0);
    soundex((char*)word,sou);
    if(bfndky(sound,(char*)sou,&i)==0){
      btsel(sound,sou,mean,1000,&i);
      buff = gtk_text_view_get_buffer(textview_result); //Returns the GtkTextBuffer being displayed by this text view
      gtk_text_buffer_set_text(buff,(gchar*)mean,strlen(mean));
      btcls(sound);
  }
  }
  
}

void match_select(GtkEntryCompletion *widget,GtkTreeModel *model,GtkTreeIter *iter,gpointer user_data){
  GValue value = {0, };
  const char *word;
  gtk_tree_model_get_value(model,iter,COL_WORD,&value);//khoi tao va thiet lap value vao COL_WORD
  word = g_value_get_string(&value);//lay noi dung chuoi trong value
  gtk_entry_set_text(GTK_ENTRY(textview_search),word);
  g_value_unset(&value);// xoa gia tri va unset kieu du lieu
}


void setup_list()
{

  GtkListStore *liststore;
  GtkTreeIter iter;
  char word[1000];
  char data[1000];
  char mean[1000];
  int i;
  completion = gtk_entry_completion_new(); //creat a new GtkEntryCompletion
  gtk_entry_completion_set_text_column(completion,COL_WORD);//ham cai dat completion de hien thi cac chuoi trong danh sach,lay cac chuoi tu column trong cac mau completion
  gtk_entry_set_completion(GTK_ENTRY(textview_search),completion);
  g_signal_connect(G_OBJECT(completion),"match-selected",G_CALLBACK(match_select),NULL);//ket noi tin hieu
  liststore = gtk_list_store_new(1,G_TYPE_STRING);
  btpos(evdata,1);
  while(btseln(evdata,word,data,1000,&i)==0){
    gtk_list_store_append(liststore,&iter);
    gtk_list_store_set(liststore,&iter,COL_WORD,word,-1);
  }
  gtk_entry_completion_set_model(completion,GTK_TREE_MODEL(liststore));//neu completion da duoc cai dat, no se remove truoc khi cai dat cai moi
}


int main (int argc, char *argv[]) {
  evdata = btopn("../data/english-vietnamese.dat",0,0);
  GtkBuilder  *builder; 
  gtk_init (&argc, &argv);
  builder = gtk_builder_new ();
  gtk_builder_add_from_file (builder, "/home/minh/myProject/cmake-gtk/dic.glade", NULL);
  window = GTK_WIDGET (gtk_builder_get_object (builder, "window"));
  find_button = GTK_WIDGET(gtk_builder_get_object (builder, "button_find"));
  textview_search = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"entry_find"));
  quit = GTK_WIDGET(gtk_builder_get_object(builder,"button_quit"));
  textview_result = GTK_TEXT_VIEW(gtk_builder_get_object(builder,"textview"));
  gtk_builder_connect_signals(builder,NULL);


  g_signal_connect(find_button, "clicked", G_CALLBACK (on_button_find_ev_clicked), textview_search);

  //thoat chuong trinh
  g_signal_connect_swapped (quit, "clicked", G_CALLBACK (gtk_main_quit), NULL);


  g_object_unref(G_OBJECT(builder));
  setup_list();
  gtk_widget_show(window);
  gtk_main();

}
