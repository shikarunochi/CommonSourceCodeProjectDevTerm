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
	switch(selectType){
	case FLOPPY_DISK_1:
		sprintf(caption,"Select DISK[1] Image File");break;
	case FLOPPY_DISK_2:
		sprintf(caption,"Select DISK[2] Image File");break;
	case CASETTE_TAPE:
		sprintf(caption,"Select TAPE Image File");break;
	case CARTRIDGE:
		sprintf(caption,"Select CARTRIDGE Image File");break;
	case QUICK_DISK:
		sprintf(caption,"Select QuickDisk Image File");break;
	default:
		return;
	}
	
    gtk_init(0, NULL);
    printf ( "GTK UI Init: %d.%d.%d\n", gtk_get_major_version ( ), gtk_get_minor_version ( ), gtk_get_micro_version ( ) );
    GtkWidget *pFileSelection = gtk_file_chooser_dialog_new(
        caption,
        NULL,
        GTK_FILE_CHOOSER_ACTION_OPEN,
        ("Cancel"), GTK_RESPONSE_CANCEL,
        ("Open"), GTK_RESPONSE_ACCEPT,
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
    if (res == GTK_RESPONSE_ACCEPT)
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
		switch(selectType){
		case FLOPPY_DISK_1:
			vm->open_floppy_disk(0, filename ,0);break;
		case FLOPPY_DISK_2:
			vm->open_floppy_disk(1, filename ,0);break;
		case CASETTE_TAPE:
            if (vm->is_tape_inserted(0)) {
            	vm->push_stop(0);
                vm->close_tape(0);
            	vm->play_tape(0, filename);
            	vm->push_play(0);
            }else{
    			vm->play_tape(0, filename);
            }
			break;
		case CARTRIDGE:
			vm->open_cart(0, filename);
			break;
		case QUICK_DISK:
			vm->open_quick_disk(0, filename);
		}
		g_free (filename);	
	}
}

void OSD::fileSelectDialog(){
	gtk_init(0, NULL);
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 	GtkWidget *dialog = gtk_dialog_new_with_buttons ("FILE SELECT",
                                       NULL,
                                       flags,
 		                               //("CANCEL"),
                                       //GTK_RESPONSE_NONE,
                                       NULL);
	#ifdef USE_FLOPPY_DISK
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "DISK 1",
                       FLOPPY_DISK_1);

	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "DISK 2",
                       FLOPPY_DISK_2);
	#endif
	#ifdef USE_TAPE 
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "TAPE",
                       CASETTE_TAPE);
	#endif
	#ifdef USE_CART
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "CARTRIDGE",
                       CARTRIDGE);
	#endif
	#ifdef USE_QUICK_DISK
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "QD",
                       QUICK_DISK);
	#endif
	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	printf("dialog result:%d\n",res);
	
	gtk_widget_destroy(dialog);

	while (gtk_events_pending ()){
        gtk_main_iteration ();
	}
	
	if(res > 0){
		selectFile(res);
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