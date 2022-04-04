/*
	Skelton for retropc emulator

	[ GTK3 Dialog ]

  	[for DevTerm]
	Author : @shikarunochi
	Date   : 2022.04.02-

*/


#include <gtk/gtk.h>
#include "osd.h"
#include "../emu.h"
#include <time.h>
#include <sys/stat.h>
#include <string.h>

//https://vivacocoa.jp/cpp/gtk3/gtk3_dialog.php
//https://vivacocoa.jp/cpp/gtk3/gtk3_layout.php
//https://blog.michinari-nukazawa.com/2016/03/gtk3-dialog-customize-add-widget.html
//http://tampo.main.jp/gtk3/chap13/chap13.html

static void saveStateCallback(GtkWidget *widget, gpointer userdata){
	SaveStatusDialogInfo *saveStatusDialogInfo = (SaveStatusDialogInfo*)userdata;
	OSD *osd;
	osd = (OSD*)(saveStatusDialogInfo->osd);
	gtk_button_set_label(GTK_BUTTON(widget),"Saving...");
	while (gtk_events_pending ()){
   	    gtk_main_iteration ();
	}
	osd->save_state(saveStatusDialogInfo->selectNo);
	if(osd->stateFileDialog != NULL){
		gtk_widget_destroy(osd->stateFileDialog);
		osd->stateFileDialog = NULL;
	}
	while (gtk_events_pending ()){
   	    gtk_main_iteration ();
	}
}

static void loadStateCallback(GtkWidget *widget, gpointer userdata){	
	SaveStatusDialogInfo *saveStatusDialogInfo = (SaveStatusDialogInfo*)userdata;
	OSD *osd;
	osd = (OSD*)(saveStatusDialogInfo->osd);
	gtk_button_set_label(GTK_BUTTON(widget),"Loading...");
	while (gtk_events_pending ()){
   	    gtk_main_iteration ();
	}
	osd->load_state(saveStatusDialogInfo->selectNo);
	if(osd->stateFileDialog != NULL){
		gtk_widget_destroy(osd->stateFileDialog);
		osd->stateFileDialog = NULL;
	}
	while (gtk_events_pending ()){
   	    gtk_main_iteration ();
	}
}

void OSD::stateFileListDialog(){
	gtk_init(0, NULL);
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 	stateFileDialog = gtk_dialog_new_with_buttons("Save Data List",
      NULL,
      flags,
      "_Cancel",
      GTK_RESPONSE_REJECT,
      NULL);

	char timeStr[128];
	GtkWidget * saveButton[10]; 
	GtkWidget * loadButton[10]; 
	SaveStatusDialogInfo saveStatusDialogInfo[10];
	GtkWidget*content=gtk_dialog_get_content_area(GTK_DIALOG(stateFileDialog));
	for(int i = 0; i < 10; i++) {
		const _TCHAR* stateFilePath = state_file_path(i);
		sprintf(timeStr,"NO DATA");
		bool isExist = false;
		if(FILEIO::IsFileExisting(stateFilePath)){
			struct stat st;
			if(stat(stateFilePath, &st) == 0) {
				time_t t = st.st_mtime;
				struct tm lt;
				localtime_r(&t, &lt);
				strftime(timeStr,sizeof(timeStr), "%Y/%m/%d %H:%M:%S", &lt);
				//printf("%s :%s\n", stateFilePath, timeStr);
				isExist = true;
			}else{
				printf("%s :get stat fail\n", stateFilePath);
			}
		}
		
		GtkWidget *stateFileInfo=gtk_box_new(GTK_ORIENTATION_HORIZONTAL,1);
		GtkWidget *stateLabel=gtk_label_new_with_mnemonic(timeStr);
		gtk_widget_set_size_request(stateLabel, 200, 25);
 		gtk_box_pack_start(GTK_BOX(stateFileInfo),stateLabel,false,false,5);

		saveButton[i] = gtk_button_new_with_label("Save");
		gtk_widget_set_size_request(saveButton[i], 100, 25);
		
		saveStatusDialogInfo[i].osd = this;
		saveStatusDialogInfo[i].selectNo = i; 
		g_signal_connect(G_OBJECT(saveButton[i]),"clicked", G_CALLBACK(saveStateCallback),(void*)&(saveStatusDialogInfo[i]));
		gtk_box_pack_end(GTK_BOX(stateFileInfo),saveButton[i],false,false,5);
		if(isExist){
			loadButton[i] = gtk_button_new_with_label("Load");
			gtk_widget_set_size_request(loadButton[i], 100, 25);
			g_signal_connect(G_OBJECT(loadButton[i]),"clicked", G_CALLBACK(loadStateCallback),(void*)&(saveStatusDialogInfo[i]));
			gtk_box_pack_start(GTK_BOX(stateFileInfo),loadButton[i],false,false,5);
 		}
		gtk_box_pack_start(GTK_BOX(content),stateFileInfo,true,true,1);
	}
	gtk_widget_show_all(stateFileDialog);
	gint result = gtk_dialog_run(GTK_DIALOG(stateFileDialog));
	if(stateFileDialog != NULL){
		gtk_widget_destroy(stateFileDialog);
	}

	while (gtk_events_pending ()){
        gtk_main_iteration ();
	}
	stateFileDialog = NULL;
}

const _TCHAR *OSD::state_file_path(int stateNo)
{
	return create_local_path(_T("%s.sta%d"), _T(CONFIG_NAME), stateNo);
}

void OSD::save_state(int stateNo){
	EMU* emu = vm->getEmu();
	emu->save_state(state_file_path(stateNo));
	printf("save State:%d\n",stateNo);
}

void OSD::load_state(int stateNo){
	EMU* emu = vm->getEmu();
	emu->load_state(state_file_path(stateNo));
	printf("load State:%d\n",stateNo);
}

int OSD::selectD88BankDialog(int drv){
#ifdef USE_FLOPPY_DISK		
	EMU* emu = vm->getEmu();
    if (emu->d88_file[drv].bank_num <= 1) {
        return 0;
    }

	gtk_init(0, NULL);
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 	GtkWidget *dialog = gtk_dialog_new_with_buttons ("BANK SELECT",
                                       NULL,
                                       flags,
 		                               //("CANCEL"),
                                       //GTK_RESPONSE_NONE,
                                       NULL);
	for(int index = 0;index < emu->d88_file[drv].bank_num;index++){
		gtk_dialog_add_button (GTK_DIALOG(dialog),
                      emu->d88_file[drv].disk_name[index],
                      index);
	}

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	printf("select d88 bank:%d\n",res);
	
	gtk_widget_destroy(dialog);

	while (gtk_events_pending ()){
        gtk_main_iteration ();
	}
	
	return res;
#else
	return 0;
#endif
}

int OSD::selectBootModePC88(){
#if defined(_PC8001) 
	return -1;
#endif	
	
	gtk_init(0, NULL);
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 	GtkWidget *dialog = gtk_dialog_new_with_buttons ("BOOT MODE",
                                       NULL,
                                       flags,
 		                               //("CANCEL"),
                                       //GTK_RESPONSE_NONE,
                                       NULL);
#if defined(_PC8801MA) 
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N88-V1(S) mode",
                       0);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N88-V1(H) mode",
                       1);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N88-V2 mode",
                       2);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N mode",
                       3);
#endif

#if defined(_PC8801MK2) || defined(_PC8801) 
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N88 mode",
                       0);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N mode",
                       1);
#endif	
	
#if defined(_PC8001SR) 
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N80 mode",
                       0);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N80(SR) mode",
                       1);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N mode",
                       2);
#endif	
#if defined(_PC8001MK2) 
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N80 mode",
                       0);
   	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "N mode",
                       1);
#endif	

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	printf("Reset mode :%d\n",res);
	
	gtk_widget_destroy(dialog);

	while (gtk_events_pending ()){
        gtk_main_iteration ();
	}
	
	if(res >= 0){
		return res;
	}
	return -1;	
}

