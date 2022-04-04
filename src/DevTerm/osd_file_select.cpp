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
#include "../emu.h"

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
	case CARTRIDGE_1:
	case CARTRIDGE_2:
		sprintf(caption,"Select CARTRIDGE Image File");break;
	case QUICK_DISK:
		sprintf(caption,"Select QuickDisk Image File");break;
	case BINARY_FILE:
		sprintf(caption,"Select Binary Image File");break;
	default:
		return;
	}
	
    gtk_init(0, NULL);
    //printf ( "GTK UI Init: %d.%d.%d\n", gtk_get_major_version ( ), gtk_get_minor_version ( ), gtk_get_micro_version ( ) );
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
		int bank_num = 0;
		switch(selectType){
		case FLOPPY_DISK_1:
			bank_num = checkD88Bank(0, filename ,0);
			if(bank_num <= 1){
				vm->open_floppy_disk(0, filename ,0);
			}else{
				int select_bank = selectD88BankDialog(0);
				if(select_bank>=0){
					vm->open_floppy_disk(0, filename ,select_bank);
				}
			}
			break;
		case FLOPPY_DISK_2:
			bank_num = checkD88Bank(1, filename ,0);
			if(bank_num <= 1){
				vm->open_floppy_disk(1, filename ,0);
			}else{
				int select_bank = selectD88BankDialog(1);
				if(select_bank>=0){
					vm->open_floppy_disk(1, filename ,select_bank);
				}
			}
			break;
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
		case CARTRIDGE_1:
			vm->open_cart(0, filename);
			break;
		case CARTRIDGE_2:
			vm->open_cart(1, filename);
			break;
		case QUICK_DISK:
			vm->open_quick_disk(0, filename);
			break;
		case BINARY_FILE:
			vm->load_binary(0, filename);
		}
		g_free (filename);	
	}else{ //cancel の場合、EJECT確認
	GtkDialogFlags flags = GTK_DIALOG_DESTROY_WITH_PARENT;
 	GtkWidget *dialog = gtk_dialog_new_with_buttons ("EJECT?",
                                       NULL,
                                       flags,
 		                               ("KEEP"),GTK_RESPONSE_NONE,
									   ("EJECT"), GTK_RESPONSE_ACCEPT,
                                       NULL);

		gint res = gtk_dialog_run(GTK_DIALOG(dialog));

		gtk_widget_destroy(dialog);

		while (gtk_events_pending ()){
			gtk_main_iteration ();
		}
    	if (res == GTK_RESPONSE_ACCEPT){
			switch(selectType){
			case FLOPPY_DISK_1:
				if(vm->is_floppy_disk_inserted(0)){
					vm->close_floppy_disk(0);
				}
				break;
			case FLOPPY_DISK_2:
				if(vm->is_floppy_disk_inserted(1)){
					vm->close_floppy_disk(1);
				}
				break;
			case CASETTE_TAPE:
				if (vm->is_tape_inserted(0)) {
					vm->push_stop(0);
					vm->close_tape(0);
				}
				break;
			case CARTRIDGE_1:
				if(vm->is_cart_inserted(0)){
					vm->close_cart(0);
				}
				break;
			case CARTRIDGE_2:
				if(vm->is_cart_inserted(1)){
					vm->close_cart(1);
				}
				break;
			case QUICK_DISK:
				vm->open_quick_disk(0, filename);
				break;
			}
			printf("Eject:%d\n",selectType);
		}
	}
}

int OSD::checkD88Bank(int drv, const _TCHAR *path, int bank) {
#ifdef USE_FLOPPY_DISK	
	EMU* emu = vm->getEmu();
	emu->d88_file[drv].bank_num = 0;
    emu->d88_file[drv].cur_bank = -1;
    if (check_file_extension(path, _T(".d88")) || check_file_extension(path, _T(".d77")) ||
        check_file_extension(path, _T(".1dd"))) {
        FILEIO *fio = new FILEIO();
        if (fio->Fopen(path, FILEIO_READ_BINARY)) {
            try {
                fio->Fseek(0, FILEIO_SEEK_END);
                uint32_t file_size = fio->Ftell(), file_offset = 0;
                while (file_offset + 0x2b0 <= file_size &&
                       emu->d88_file[drv].bank_num < MAX_D88_BANKS) {
                    fio->Fseek(file_offset, FILEIO_SEEK_SET);
//#ifdef _UNICODE
                    char tmp[18];
                    fio->Fread(tmp, 17, 1);
                    tmp[17] = 0;
                    convertUTF8fromSJIS(tmp,
                                        emu->d88_file[drv].disk_name[emu->d88_file[drv].bank_num],
                                        17 * 3);
//#else
//                    fio->Fread(emu->d88_file[drv].disk_name[emu->d88_file[drv].bank_num], 17, 1);
//                    emu->d88_file[drv].disk_name[emu->d88_file[drv].bank_num][17] = 0;
//#endif
                    fio->Fseek(file_offset + 0x1c, SEEK_SET);
                    file_offset += fio->FgetUint32_LE();
                    emu->d88_file[drv].bank_num++;
                }
                my_tcscpy_s(emu->d88_file[drv].path, _MAX_PATH, path);
                emu->d88_file[drv].cur_bank = bank;
            } catch (...) {
                emu->d88_file[drv].bank_num = 0;
            }
            fio->Fclose();
        }
        delete fio;
    }
	printf("bankNum:%d\n", emu->d88_file[drv].bank_num);
					
	return emu->d88_file[drv].bank_num;
#else
	return 0;
#endif

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
	if( USE_CART > 1){
		gtk_dialog_add_button (GTK_DIALOG(dialog),
	                       "CARTRIDGE 1",
	                       CARTRIDGE_1);
		gtk_dialog_add_button (GTK_DIALOG(dialog),
	                       "CARTRIDGE 2",
	                       CARTRIDGE_2);

	}else{
		gtk_dialog_add_button (GTK_DIALOG(dialog),
	                       "CARTRIDGE",
	                       CARTRIDGE_1);
	}		
	#endif
	#ifdef USE_QUICK_DISK
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "QD",
                       QUICK_DISK);
	#endif
	#ifdef USE_BINARY_FILE
	gtk_dialog_add_button (GTK_DIALOG(dialog),
                       "BINARY",
                       BINARY_FILE);
	#endif

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));
	//printf("dialog result:%d\n",res);
	
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