/*
	Skelton for retropc emulator

	[ file select ]

  	[for DevTerm]
	Author : @shikarunochi
	Date   : 2022.02.01-

*/

//https://docs.gtk.org/gtk3/class.FileChooserDialog.html
#include <gtk/gtk.h>
#include "osd.h"

//https://bbs.archlinux.org/viewtopic.php?id=101622
//https://www.exodabe.com/20170806488.html
void OSD::selectFile(int selectType){
	char caption[128];
	if(selectType == 1){
		sprintf(caption,"Select Tape Image File");
	}else if(selectType == 2){
		sprintf(caption,"Select DISK:1 Image File");
	}else{
		sprintf(caption,"Select Cartridge Image File");
	}
	
    gtk_init(0, NULL);
    printf ( "GTK UI Init: %d.%d.%d\n", gtk_get_major_version ( ), gtk_get_minor_version ( ), gtk_get_micro_version ( ) );
    GtkWidget *pFileSelection = gtk_file_chooser_dialog_new(
        caption,
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
        GTK_STOCK_OPEN, GTK_RESPONSE_OK,
        NULL
    );
	//gtk_window_set_resizable  (GTK_WINDOW (pFileSelection), TRUE);
    //gtk_widget_set_size_request (pFileSelection, 800, 400);
/*
	GtkFileFilter *filter = gtk_file_filter_new();

	if(selectType == 1){
		gtk_file_filter_set_name(filter, "All Files");
	    gtk_file_filter_add_pattern(filter, "*");
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pFileSelection), filter);
	}else{
		gtk_file_filter_set_name(filter, "All Files");
	    gtk_file_filter_add_pattern(filter, "*");
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(pFileSelection), filter);
	}
*/	
	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER (pFileSelection),get_application_path());
	
	gint res = gtk_dialog_run(GTK_DIALOG(pFileSelection));
	char *filename = NULL;
    if (res == GTK_RESPONSE_OK)
    {
        GtkFileChooser *chooser = GTK_FILE_CHOOSER (pFileSelection);
        filename = gtk_file_chooser_get_filename (chooser);
    }	

	gtk_widget_destroy(pFileSelection);

	while (gtk_events_pending ()){
        gtk_main_iteration ();
	}
	if(filename != NULL){
        printf("Select: %s\n",filename);
		if(selectType == 1){
            if (vm->is_tape_inserted(0)) {
            	vm->push_stop(0);
                vm->close_tape(0);
            	vm->play_tape(0, filename);
            	vm->push_play(0);
            }else{
    			vm->play_tape(0, filename);
            }
		}else if(selectType == 2){
			vm->open_floppy_disk(0, filename ,0);
		}else{
			vm->open_cart(0, filename);
		}
		g_free (filename);	
	}
}

/*
void OSD::selectFile(int fileType){
	//ファイル一覧取得
	const _TCHAR *appPath = get_application_path();
	DIR *dir;
    struct dirent *dp;
	
    dir = opendir(appPath);
    if (dir == NULL) {
        return;
    }
	
    while (1) {
        dp = readdir(dir);
        if (dp == NULL) {
            break;
        }
        printf("%s\n", dp->d_name);
    }
    closedir(dir);
	
}
*/