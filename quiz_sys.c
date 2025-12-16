// This project exists to learn C deeply
// It is intentionally overbuilt, non-portable, and has some "human" touches also if i had more time i would have added MD5 hashing for passwords and answer files
// dont use scanf for input handling, modularity is key, C is unforgiving, NEVER USE GLOBALS
#include <stdio.h>
#include <windows.h>
#include <math.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
float version = 1.00;
struct quiz_index {
    int physics,maths,c_prog,linux,prob,global_tmp;
} ndx;
struct logged_in_user {
    int marks,maximum_marks;
    char username[100],name[100],batch[5],SAP_id[10];
} student_details;
struct quiz_details{
    int subject_index,number_of_questions;
    char quizz_name[100],quizz_filename[100],subject[20];
} quizz_details;
struct{
    int positive_marks,negative_marks;
    char question[100],optionA[50],optionB[50],optionC[50],optionD[50],picked_option;
} stu_quizz;
struct analytic_tmp{
    int correct_question_numbers[100],correct_QN_index,incorrect_QN_index, skipped_QN_index, incorrect_question_numbers[100], skipped_question_numbers[100], positive_marks, negative_markz;
    char question_tmp[100], tmp_optA[100], tmp_optB[100], tmp_optC[100], tmp_optD[100], correct_option, response;
}analytics_tmp;


int student_validity();
void user_setup(char *sap, char *name_with_underscores);
int student_dashboard();
int stu_subject_panel(char mode);
int quizz_take();
int evaluator();

int user_database();
int database(char identity, struct quiz_details *qd);
//int performance_analytics(char); In-deapth analytics work in progress

int admin_check();
void admin_mode();
int make_quiz();
int mcq_make();

void clearInputBuffer(){
    int b;
    while((b = getchar()) != '\n' && b != EOF);
}
char safe_option_extract();
int safe_num_extract(int);
void replace_space(char input [], char output []);
void replace_underscore(char input [], char output []);
int folder_exists(char *path);
void safe_fgets(char *buf, int size);
void gated_string_input(char string[],int length, int need_cords, int x, int y, int is_concealed, int force_lowercase, int allow_space, int next_line);
int verify_integrity();
void fetch_index();
void append_index(char []);
void pretty_little_loading_bar();
void fast_little_loading_bar(int length,char symbol);
void scary_little_loading_bar(int,int,char);
void press_enter();
void get_number_of_questions();
void cute_box(int rows,int columbs,char top,char end, char borders[]);
void gotoXY(int row, int col) { 
    printf("\033[%d;%dH", row, col);    //move to desired row and column via ansi escape and coordinates
}
int numeric_check(const char *in);
void clearLine(int row){
    printf("\033[%d:1H\033[2K", row);
}

//WINDOWS API FUNCTIONS & UI
void print_menu_row(char *line, int row, int column, int is_highlight, char *color);
void menu_fix_logic(int *menu_select, int *row_trk, int *sanity, int bottom_limit_coordinate, int top_limit_coordinate, int horizontal_correction);
void hide_cursor();
void show_cursor();
void print_arrow(int row, int column);
void green() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
}
void red() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
}
void yellow() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
}
void blue() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 1);
}
void dark_grey() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
}
void reset(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void WIP(){
    red();
    printf("\nWork in progress....\n");
    reset();
    press_enter();
    system("cls");
}


//int FITB();

int main(){
    //int menu_select; NOT SAFE debug only
    hide_cursor();
    int menu_select;
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("cls");
    verify_integrity();
    system("cls");
    
    while(1){
        hide_cursor();
        int sanity = 0, row_trk;
        red();
        gotoXY(1,1);
        fast_little_loading_bar(25, '.');
        printf("\nMAIN MENU\n");
        fast_little_loading_bar(25, '.');
        // dark_grey();
        // printf("\n  Admin mode\n  Student\n  Exit");
        row_trk = 4;
        gotoXY(4,1);
        red();
        printf("->");
        reset();
        while(sanity != 1){
            print_menu_row("Student", 4, 3, row_trk == 4, "blue");
            print_menu_row("Admin", 5, 3, row_trk == 5, "blue");
            print_menu_row("EXIT", 6, 3, row_trk == 6, "red");
            
            menu_fix_logic(&menu_select, &row_trk, &sanity, 6, 4, 1);
            
        }
        reset(); 
        show_cursor();
        
        
        //menu_select = safe_num_extract(2);
        system("cls");
        if (menu_select == 1){
            int ID_verify = student_validity();
            if(ID_verify == 1){
                green();
                scary_little_loading_bar(30, 10, '=');
                reset();
                system("cls");
                student_dashboard();
                system("cls");
            }else if(ID_verify == 2){
                red();
                printf("Incorrect Username or Password");
                reset();
                Sleep(2000);
                system("cls");
                continue;
            }
        }else if(menu_select == 2){
            int is_admin = admin_check();
            if(is_admin == 1){
                green();
                printf("\n");
                scary_little_loading_bar(20, 20, '#');
                reset();
                admin_mode();
                system("cls");
            }else if(is_admin == 2){
                red();
                printf("\nIncorrect Username or Password\n");
                reset();
                Sleep(2000);
                system("cls");
            continue;
            }
        }else if(menu_select == 3){
            red();
            printf("\nExiting");
            pretty_little_loading_bar();
            reset();
        return 0;
        }
    }
}

//General functions
void safe_fgets(char *buf, int size){
    
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = '\0';

}
void gated_string_input(char string [], int length,int need_cords, int row, int column, int is_concealed, int force_lowercase, int allow_space, int next_line){
    char tmp, i = 0;
    length++;
    if(need_cords == 1){
        gotoXY(row, column);
    }
    while(1){
        tmp = getch();
        if(tmp == ' ' && allow_space == 0){
            continue;
        }
        if(tmp == '\r' && i == 0){
            continue;
        }
        if(tmp == '\r' && i > 0){
            break;
        }
        if(tmp == '\b' && i > 0){
            string[i] = '\0';
            i--;
            printf("\b \b");
            continue;
        }
        if(tmp == '\b' && i == 0){
            continue;
        }
        if(force_lowercase == 0){
            if(tmp >= 32 && tmp <= 126 && i != (length-1)){
                string[i] = tmp;
                if(is_concealed == 0){
                    printf("%c", string[i]);
                }else if(is_concealed == 1){
                    printf("*");
                }
                i++;
            }
        }else if(force_lowercase == 1){
            if(tmp >= 97 && tmp <= 122 && i != (length-1)){
                string[i] = tmp;
                if(is_concealed == 0){
                    printf("%c", string[i]);
                }else if(is_concealed == 1){
                    printf("*");
                }
                i++;
            }
        }
    }   string[i] = '\0';
    if(next_line == 1){
        printf("\n");
    }
}
int folder_exists(char *path){
    DWORD check = GetFileAttributesA(path);
    return(check != INVALID_FILE_ATTRIBUTES && (check & FILE_ATTRIBUTE_DIRECTORY));
}
void hide_cursor(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
void show_cursor(){
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;

    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = TRUE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
}
int verify_integrity(){
    FILE *first_run = fopen("build.txt","r");
    if(first_run == NULL){
        printf("Running initial setup");
        pretty_little_loading_bar();
        CreateDirectory("credentials", NULL);
        CreateDirectory("index", NULL);
        CreateDirectory("quizzes", NULL);
        CreateDirectory("quizzes/physics", NULL);
        CreateDirectory("quizzes/physics/answers", NULL);
        CreateDirectory("quizzes/maths", NULL);
        CreateDirectory("quizzes/maths/answers", NULL);
        CreateDirectory("quizzes/c_prog", NULL);
        CreateDirectory("quizzes/c_prog/answers", NULL);
        CreateDirectory("quizzes/linux", NULL);
        CreateDirectory("quizzes/linux/answers", NULL);
        CreateDirectory("quizzes/prob_solving", NULL);
        CreateDirectory("quizzes/prob_solving/answers", NULL);
        CreateDirectory("analytics", NULL);
        CreateDirectory("analytics/physics", NULL);
        CreateDirectory("analytics/maths", NULL);
        CreateDirectory("analytics/c_prog", NULL);
        CreateDirectory("analytics/linux", NULL);
        CreateDirectory("analytics/prob_solving", NULL);
        CreateDirectory("analytics/physics/B7", NULL);
        CreateDirectory("analytics/maths/B7", NULL);
        CreateDirectory("analytics/c_prog/B7", NULL);
        CreateDirectory("analytics/linux/B7", NULL);
        CreateDirectory("analytics/prob_solving/B7", NULL);
        CreateDirectory("responses",NULL);
        CreateDirectory("responses/physics", NULL);
        CreateDirectory("responses/physics/B7", NULL);
        CreateDirectory("responses/maths", NULL);
        CreateDirectory("responses/maths/B7", NULL);
        CreateDirectory("responses/c_prog", NULL);
        CreateDirectory("responses/c_prog/B7", NULL);
        CreateDirectory("responses/linux", NULL);
        CreateDirectory("responses/linux/B7", NULL);
        CreateDirectory("responses/prob_solving", NULL);
        CreateDirectory("responses/prob_solving/B7", NULL);

        FILE *build = fopen("build.txt","w");
        fprintf(build, "%.2f",version);
        fclose(build);
        FILE *ndx = fopen("index/index.txt","w");
        fprintf(ndx, "0 0 0 0 0");
        fclose(ndx);
        FILE *cred = fopen("credentials/admin.txt","w");
        fprintf(cred,"admin admin123 Hridyansh");
        fclose(cred);
        FILE *Cred = fopen("credentials/user.txt","w");
        fprintf(Cred,"gautam 123 Hridyansh 590021474 B7");
        fclose(Cred);
        FILE *User_count = fopen("index/user_count.txt","w");
        fprintf(User_count,"1");
        fclose(User_count);
        green();
        printf("\nSuccess");
        reset();
        
    }else{
        fclose(first_run);
    }

    printf("\nVerifying file structure");
    pretty_little_loading_bar();
    FILE *Ucred = fopen("credentials/user.txt","r");
    FILE *Acred = fopen("credentials/admin.txt","r");
    FILE *ndx = fopen("index/index.txt", "r");
    if(folder_exists("credentials") && folder_exists("index")  && folder_exists("quizzes") && folder_exists("quizzes/physics/answers") 
    && folder_exists("quizzes/maths/answers") && folder_exists("quizzes/c_prog/answers") && folder_exists("quizzes/linux/answers") 
    && folder_exists("quizzes/prob_solving/answers") && folder_exists("analytics") && folder_exists("analytics/physics") 
    && folder_exists("analytics/maths") && folder_exists("analytics/c_prog") && folder_exists("analytics/linux") 
    && folder_exists("analytics/prob_solving") && folder_exists("analytics/physics/B7") && folder_exists("analytics/maths/B7") 
    && folder_exists("analytics/c_prog/B7") && folder_exists("analytics/linux/B7") && folder_exists("analytics/prob_solving/B7") 
    && folder_exists("responses") && folder_exists("responses/physics") && folder_exists("responses/maths") 
    && folder_exists("responses/c_prog") && folder_exists("responses/linux") && folder_exists("responses/prob_solving") 
    && folder_exists("quizzes/physics") && folder_exists("quizzes/maths") && folder_exists("quizzes/c_prog") && folder_exists("quizzes/linux") 
    && folder_exists("quizzes/prob_solving") && Ucred != NULL && Acred != NULL && ndx != NULL){
        fclose(Ucred);
        fclose(Acred);
        fclose(ndx);
        green();
        printf("\xE2\x9C\x93");
        reset();
    }else{
        fclose(Ucred);
        fclose(Acred);
        fclose(ndx);
        yellow();
        printf("\nError some folders are missing, repairing");
        pretty_little_loading_bar();
        Sleep(4000);
        CreateDirectory("credentials", NULL);
        CreateDirectory("index", NULL);
        CreateDirectory("quizzes", NULL);
        CreateDirectory("quizzes/physics", NULL);
        CreateDirectory("quizzes/physics/answers", NULL);
        CreateDirectory("quizzes/maths", NULL);
        CreateDirectory("quizzes/maths/answers", NULL);
        CreateDirectory("quizzes/c_prog", NULL);
        CreateDirectory("quizzes/c_prog/answers", NULL);
        CreateDirectory("quizzes/linux", NULL);
        CreateDirectory("quizzes/linux/answers", NULL);
        CreateDirectory("quizzes/prob_solving", NULL);
        CreateDirectory("quizzes/prob_solving/answers", NULL);
        CreateDirectory("analytics", NULL);
        CreateDirectory("analytics/physics", NULL);
        CreateDirectory("analytics/maths", NULL);
        CreateDirectory("analytics/c_prog", NULL);
        CreateDirectory("analytics/linux", NULL);
        CreateDirectory("analytics/prob_solving", NULL);
        CreateDirectory("analytics/physics/B7", NULL);
        CreateDirectory("analytics/maths/B7", NULL);
        CreateDirectory("analytics/c_prog/B7", NULL);
        CreateDirectory("analytics/linux/B7", NULL);
        CreateDirectory("analytics/prob_solving/B7", NULL);
        CreateDirectory("responses",NULL);
        CreateDirectory("responses/physics", NULL);
        CreateDirectory("responses/physics/B7", NULL);
        CreateDirectory("responses/maths", NULL);
        CreateDirectory("responses/maths/B7", NULL);
        CreateDirectory("responses/c_prog", NULL);
        CreateDirectory("responses/c_prog/B7", NULL);
        CreateDirectory("responses/linux", NULL);
        CreateDirectory("responses/linux/B7", NULL);
        CreateDirectory("responses/prob_solving", NULL);
        CreateDirectory("responses/prob_solving/B7", NULL);
        FILE *ucred = fopen("credentials/user.txt","r");
        FILE *acred = fopen("credentials/admin.txt","r");
        FILE *Ndx = fopen("index/index.txt", "r");
        if(ucred == NULL){ //Will add advance recover soon                                                                          TBD
            red();
            printf("\n\nCRITICAL ERROR! USER CREDENTIAL DATA NOT FOUND!\nPress Enter to reset to default this can cause data loss......");
            char tmp;
            while(1){
                tmp = getch();
                if(tmp == '\r'){
                    break;
                }
            }
            green();
            printf("\xE2\x9C\x93");
            FILE *uucred = fopen("credentials/user.txt","w");
            fprintf(uucred,"gautam 123 Hridyansh 590021474 B7");
            fclose(uucred);
            green();
            Sleep(2000);
            printf("\nUser data has been reset to default...\n");
        }else{
            fclose(ucred);
        }
        if(acred == NULL){ //Will add advance recover soon                                                                          TBD
            red();
            printf("\nCRITICAL ERROR! ADMIN CREDENTIAL DATA NOT FOUND!\nPress Enter to reset to default this can cause data loss......");
            char tmp;
            while(1){
                tmp = getch();
                if(tmp == '\r'){
                    break;
                }
            }
            green();
            printf("\xE2\x9C\x93");
            FILE *aacred = fopen("credentials/admin.txt","w");
            fprintf(aacred,"admin admin123 Hridyansh");
            fclose(aacred);
            green();
            Sleep(2000);
            printf("\nAdmin data has been reset to default...");
        }else{
            fclose(acred);
        }
        if(Ndx == NULL){
            red();
            printf("\n\nCRITICAL ERROR! INDEX DATA NOT FOUND!\nPress Enter to attempt repair to prevent permanent data loss......");
            char tmp;
            while(1){
                tmp = getch();
                if(tmp == '\r'){
                    break;
                }
            }
            green();
            printf("\xE2\x9C\x93");
            reset();
            int physics_index, maths_index, c_index, linux_index, prob_solving_index;
            int ip = 1, im = 1, ic = 1, il = 1, ips = 1;
            char physics_file_path[100], maths_file_path[100], c_file_path[100], linux_file_path[100], prob_solving_file_path[100];
        
            while(1){
                sprintf(physics_file_path,"quizzes/physics/physics_quizz_%d.txt", ip);
                FILE *physics = fopen(physics_file_path, "r");
                if(physics == NULL){
                    if(ip == 1){
                        physics_index = 0;
                        break;
                    }
                    physics_index = (ip-1);
                    break;
                }
                fclose(physics);
                ip++;
            }
            while(1){
                sprintf(maths_file_path,"quizzes/maths/maths_quizz_%d.txt", im);
                FILE *maths = fopen(maths_file_path, "r");
                if(maths == NULL){
                    if(im == 1){
                        maths_index = 0;
                        break;
                    }
                    maths_index = (im-1);
                    break;
                }
                fclose(maths);
                im++;
            }
            while(1){
                sprintf(c_file_path,"quizzes/c_prog/c_prog_quizz_%d.txt", ic);
                FILE *C_prog = fopen(c_file_path, "r");
                if(C_prog == NULL){
                    if(ic == 1){
                        c_index = 0;
                        break;
                    }
                    c_index = (ic-1);
                    break;
                }
                fclose(C_prog);
                ic++;
            }
            while(1){
                sprintf(linux_file_path,"quizzes/linux/linux_quizz_%d.txt", il);
                FILE *linux = fopen(linux_file_path, "r");
                if(linux == NULL){
                    if(il == 1){
                        linux_index = 0;
                        break;
                    }
                    linux_index = (il-1);
                    break;
                }
                fclose(linux);
                il++;
            }
            while(1){
                sprintf(maths_file_path,"quizzes/prob_solving/prob_solving_quizz_%d.txt", ips);
                FILE *prob_solving = fopen(prob_solving_file_path, "r");
                if(prob_solving == NULL){
                    if(ips == 1){
                        prob_solving_index = 0;
                        break;
                    }
                    prob_solving_index = (ips-1);
                    break;
                }
                fclose(prob_solving);
                ips++;
            }
            Sleep(2000);
            if(physics_index > 0 || maths_index > 0 || c_index > 0 || linux_index > 0 || prob_solving_index > 0){
                system("cls");
                red();
                printf("RECOVERED QUIZZ INDEX\n->Physics: %d\n->Maths: %d\n->C Programming: %d\n->Linux: %d\n->Problem Solving: %d\n", physics_index, maths_index, c_index, linux_index, prob_solving_index);
                reset();
                press_enter();
                FILE *nnndx = fopen("index/index.txt", "w");
                fprintf(nnndx, "%d %d %d %d %d", physics_index, maths_index, c_index, linux_index, prob_solving_index);
                fclose(nnndx);
            }else{
                printf("RECOVERY FAILED! NO INDEX DATA FOUND\n");
                FILE *nndx = fopen("index/index.txt","w");
                fprintf(nndx,"0 0 0 0 0");
                fclose(nndx);
                yellow();
                Sleep(2000);
                printf("INDEX data has been reset to default..."); 
            }
            
            
            
        }else{
            fclose(Ndx);
        }
        green();
        printf("\xE2\x9C\x93");
        reset();
    }
    Sleep(3000);
    system("cls");
    green();
    printf("\nSystem ready");
    reset();
    press_enter();
    return 1;
}
void pretty_little_loading_bar(){
    for(int i = 0; i < 10; i++){
            printf(".");
            Sleep(100);
    }
}
void scary_little_loading_bar(int length,int delay, char symbol){
    for(int i = 1; i <= length; i++){
        printf("%c", symbol);
        Sleep(delay);
    }
}
void fast_little_loading_bar(int length, char symbol){
    for(int i = 0; i < length; i++){
        printf("%c", symbol);
    }
}
void press_enter(){
    char tmp;
    printf("\nPress enter to continue....");
    while(1){
        tmp = getch();
        if(tmp == '\r'){
            break;
        }
    }
}

char safe_option_extract(){
    char in, choice;
    int sanity = 0;
    while(1){
        in = getch();
        
        if(sanity == 1 && in == '\r'){
            break;
        }
        if(sanity == 0 && in == '\r'){
            continue;
        }
        if(sanity == 1 && in == '\b'){
            printf("\b \b");
            sanity = 0;
            continue;
        }
        if (sanity == 0 && (in == 'a' || in == 'b' || in == 'c' || in == 'd' || in == 's')){ // 's' needs to be dissabled for quizz                                                                         TBD
            sanity = 1;
            choice = in;
            printf("%c", choice);
            continue;
        }
    }
    
    return choice;
}
int safe_num_extract(int num){
    int tmp;
    int choice, sanity = 0;
    while(1){
        tmp = getch();
        if(tmp == '\r' && sanity == 1){
            break;
        }
        if(tmp >= '0' && tmp <= '0'+num && sanity == 0){
            sanity = 1;
            choice = tmp - '0';
            printf("%c", tmp);
            continue;
        }
        if (tmp == '\b' && sanity != 0){
            sanity = 0;
            printf("\b \b");
            continue;
        }
    }
    // printf("%d", choice); DEBUG LINE
    // press_enter();
    printf("\n");
    return choice;
}
void replace_space(char input [], char output []){
    strcpy(output, input);
    for(int i = 0; output[i] != '\0'; i++){
        if(output[i] == ' '){
            output[i] = '_';
            continue;
        }
    }
}
void replace_underscore(char input [], char output []){
    strcpy(output, input);
    for(int i = 0; output[i] != '\0'; i++){
        if(output[i] == '_'){
            output[i] = ' ';
            continue; //looks cool
        }
    }
}
void fetch_index(){
    FILE *num_trk = fopen("index/index.txt","r");
    if (num_trk == NULL){
        printf("ERROR! index.txt failed to initialise");
    }
    fscanf(num_trk,"%d %d %d %d %d",&ndx.physics,&ndx.maths,&ndx.c_prog,&ndx.linux,&ndx.prob);
    fclose(num_trk);
}
void append_index(char subject[]){
    fetch_index();
    FILE *num_trk = fopen("index/index.txt","w");
    if (num_trk == NULL){
        printf("ERROR! index.txt failed to initialise");
        press_enter();
    }
    
    if(strcmp(subject,"physics")==0){
        fprintf(num_trk,"%d %d %d %d %d",ndx.physics+1,ndx.maths,ndx.c_prog,ndx.linux,ndx.prob);
    }else if(strcmp(subject,"maths")==0){
        fprintf(num_trk,"%d %d %d %d %d",ndx.physics,ndx.maths+1,ndx.c_prog,ndx.linux,ndx.prob);
    }else if(strcmp(subject,"linux")==0){  
        fprintf(num_trk,"%d %d %d %d %d",ndx.physics,ndx.maths,ndx.c_prog,ndx.linux+1,ndx.prob);
    }else if(strcmp(subject,"c_prog")==0){    
        fprintf(num_trk,"%d %d %d %d %d",ndx.physics,ndx.maths,ndx.c_prog+1,ndx.linux,ndx.prob);
    }else if(strcmp(subject,"prob_solving")==0){    
        fprintf(num_trk,"%d %d %d %d %d",ndx.physics,ndx.maths,ndx.c_prog,ndx.linux,ndx.prob+1);
    }    
    fclose(num_trk);
    fetch_index();
}
void get_number_of_questions(){
    char line[200];
    FILE *MCQ = fopen(quizz_details.quizz_filename,"r");
    if(MCQ == NULL) return;

    while(fgets(line,sizeof(line),MCQ)){
        char label[100];
        int num;
        if (sscanf(line, "%s %d", label, &num) == 2) {
            if(strcmp(label,"TOTAL_QUESTIONS") == 0){
                quizz_details.number_of_questions = num;
                break;
            }
        }
    }

    fclose(MCQ);
}
void cute_box(int verical,int horizontal,char top, char end, char borders[]){
    fast_little_loading_bar(horizontal + 4, top);
    printf("\n");
    for(int i = 1; i <= verical; i++){
        printf("%s", borders);
        for(int i = 1; i <= horizontal; i++){
            printf(" ");
        }
        printf("%s\n", borders);
    }
    fast_little_loading_bar(horizontal + 4, top);
}
void print_menu_row(char *line, int row, int column, int is_highlight, char *color){
    gotoXY(row,column);
    if(is_highlight == 1){
        if(strcmp(color, "red") == 0){
            red();
        }else if(strcmp(color, "yellow") == 0){
            yellow();
        }else if(strcmp(color, "blue") == 0){
            blue();
        }else if(strcmp(color, "green") == 0){
            green();
        }else if(strcmp(color, "dark_grey") == 0){
            dark_grey();   
        }
        printf("%s", line);
        reset();
    }else{
        dark_grey();
        printf("%s", line);
        reset();
    }
    
}
void menu_fix_logic(int *menu_select, int *row_trk, int *sanity, int bottom_limit_coordinate, int top_limit_coordinate, int horizontal_correction){
    int key, up = 72, down = 80, left = 75, right = 77;
    
        key = getch();
            if(key == '\r'){
                *menu_select = *row_trk- (top_limit_coordinate-1);
                *sanity = 1;
                return;
            }
            red();
            if(key > '0' && key <= (bottom_limit_coordinate+1) - top_limit_coordinate + '0'){
                gotoXY(*row_trk, horizontal_correction);
                printf("  ");
                *row_trk = (key - '0') + (top_limit_coordinate-1);
                gotoXY(*row_trk, horizontal_correction);
                printf("->");
            }
            if(key == 224 || key == 0){
                key = getch();
                if(key == up && *row_trk > top_limit_coordinate){
                    gotoXY(*row_trk,horizontal_correction);
                    printf("  ");
                    (*row_trk)--;
                    printf("%d", *row_trk);
                    gotoXY(*row_trk, horizontal_correction);
                    printf("->");
                }
                if(key == down && *row_trk < bottom_limit_coordinate){
                    gotoXY(*row_trk,horizontal_correction);
                    printf("  ");
                    (*row_trk)++;
                    gotoXY(*row_trk, horizontal_correction);
                    printf("->");
                }
            }
}
void print_arrow(int row, int column){
    gotoXY(row, column);
    red();
    printf("->");
    reset();
}

int numeric_check(const char *in){
    for(int i = 0; i < strlen(in); i++){
        if(in[i] < '0' || in[i] > '9'){
            return 0;
        }
    }
    return 1;
}
//Student portal funtioins
int student_validity(){
    system("cls");
    FILE *credentiaals = fopen("credentials/user.txt","r");
    if (credentiaals == NULL){
        yellow();
        printf("\nERROR! Critical credential file failed to initialise\n");
        printf("Not safe to continue\n");
        reset();
        press_enter();
        exit(0);
    }
    int i=0, sanity = 0;
    char buffer_line[100], usr[20],file_usr[20],pass[20],file_pass[20],tmp,name[20],sap[10],batch[5];
    red();
    cute_box(3, 49, '=', '=', "||");
    printf("\033[2;15HSTUDENT LOGIN CONTROL\033[3;3HUSERNAME or SAP ID:\033[4;3HPASSWORD:");
    dark_grey();
    gated_string_input(usr, 20, 1, 3, 23, 0, 0, 0, 0);
    while(fgets(buffer_line, sizeof(buffer_line), credentiaals) != NULL){
        buffer_line[strcspn(buffer_line, "\n")] = '\0';

        sscanf(buffer_line,"%s %s %s %s %s", file_usr, file_pass, name, sap, batch);
        if(numeric_check(usr)){
            if(strcmp(usr, sap) == 0 && strcmp(file_usr, "setup") == 0){
                yellow();
                printf("\033[6;1HSetup not complete press enter to begin setup.....");
                while(getchar() != '\n');
                fclose(credentiaals);
                user_setup(sap, name);
            }
        }
    }fclose(credentiaals);
    gated_string_input(pass, 15, 1, 4, 13, 1, 0, 0, 1);
    if(strcmp(usr, "setup") == 0 || strcmp(pass, "setup") == 0){
        green();
        printf("\nNice try Didddy");
        Sleep(5000); //Take this time to think of your life choices you pesky exploiter
        system("cls");
        red();
        printf("Exiting");
        pretty_little_loading_bar();
        reset();
        exit(0);//goodbye
    }
    printf("\n");
    FILE *credentials = fopen("credentials/user.txt", "r");
    while(fgets(buffer_line, sizeof(buffer_line), credentials) != NULL){
        
        buffer_line[strcspn(buffer_line,"\n")] = '\0';
        sscanf(buffer_line,"%s %s %s %s %s", file_usr, file_pass, name, sap, batch);        

        if(strcmp(sap, usr) == 0 && strcmp(file_pass, pass) == 0){
            replace_underscore(name, student_details.name);
            strcpy(student_details.username,usr);
            strcpy(student_details.SAP_id,sap);
            strcpy(student_details.batch,batch);
            fclose(credentials);
            return 1; 
        }
        
        if(strcmp(file_usr,usr)==0 && strcmp(file_pass,pass)==0){
        replace_underscore(name, student_details.name);
        strcpy(student_details.username,usr);
        strcpy(student_details.SAP_id,sap);
        strcpy(student_details.batch,batch);
        fclose(credentials);
        return 1;
        }
    }
    fclose(credentials);
    return 2;
}
void user_setup(char *sap, char *name_with_underscores){
    system("cls");
    char username[21], password[31], old_file_usr[30], old_file_pass[40], old_file_name[50], old_file_sap[12], old_file_batch[3], name[40], old_file_scan_line[100], new_file_paste_line[100];
    replace_underscore(name_with_underscores, name);

    gotoXY(1,1);
    red();
    cute_box(5, 50, '=', '=', "||");
    gotoXY(3, 3);
    fast_little_loading_bar(50, '=');
    printf("\033[2;15HCREDENTIAL CONTROL PANNEL\033[4;3HNAME:\033[5;3HUSERNAME:\033[6;3HPASSWORD:");
    blue();
    printf("\033[4;9H%s", name);
    dark_grey();
    gated_string_input(username, 20, 1, 5, 13, 0, 0, 0, 0);
    gated_string_input(password, 30, 1, 6, 13, 1, 0, 0, 0);
    
    FILE *old = fopen("credentials/user.txt", "r");
    FILE *new = fopen("credentials/temp.txt", "w");
    if(old == NULL || new == NULL){
        system("cls");
        red();
        printf("\n ERROR! Failed to initialise critical credential files\n NOT SAFE to continue......");
        while(getchar() != '\n');
        reset();
        exit(0);
    }

    while(fgets(old_file_scan_line, sizeof(old_file_scan_line), old) != NULL){
        old_file_scan_line[strcspn(old_file_scan_line, "\n")] = '\0';
        sscanf(old_file_scan_line, "%s %s %s %s %s", old_file_usr, old_file_pass, old_file_name, old_file_sap, old_file_batch);
        
        if(strcmp(old_file_sap, sap) == 0 && strcmp(old_file_usr, "setup") == 0){
            fprintf(new, "%s %s %s %s %s\n", username, password, old_file_name, old_file_sap, old_file_batch);
        }else{
            fprintf(new, "%s %s %s %s %s\n", old_file_usr, old_file_pass, old_file_name, old_file_sap, old_file_batch);
        }
    }
    fclose(old);
    fclose(new);
    green();
    printf("\033[8;1HSetup Completed Succesfully......\n");
    press_enter();
    remove("credentials/user.txt");
    rename("credentials/temp.txt", "credentials/user.txt");
    student_validity();
}

int student_dashboard(){
    
    int usr_mode, sanity, row_trk;
    char name[20], func_choice;
    strcpy(name, student_details.name);
    
    while(1){
        red();
        cute_box(4, 50, '=', '=', "||");
        yellow();
        printf("\033[2;3HWelcome %s, pleaase select a function:",name);
        //usr_mode = safe_num_extract(2);
        print_arrow(3,3);
        sanity = 0; row_trk = 3;
        hide_cursor();
        while(sanity != 1){

            print_menu_row("Quizzes", 3, 5, row_trk == 3, "blue");
            print_menu_row("Performance", 4, 5, row_trk == 4, "blue");
            print_menu_row("EXIT", 5, 5, row_trk == 5, "red");
        
            menu_fix_logic(&usr_mode, &row_trk, &sanity, 5, 3, 3);
        }show_cursor(); printf("\n");
        
        
        if (usr_mode == 1){
            yellow();
            printf("\nLoading quiz board");
            pretty_little_loading_bar();
            reset();
            func_choice = 'q';
            system("cls");
            stu_subject_panel(func_choice);
        }else if(usr_mode == 2){
            yellow();
            printf("\nLoading performance board");
            pretty_little_loading_bar();
            reset();
            system("cls");
            func_choice = 'p';
            stu_subject_panel(func_choice);
        }else if(usr_mode == 3){
            yellow();
            printf("\nLogging out");
            pretty_little_loading_bar();
            reset();
            return 1;
        }
    }
}
int stu_subject_panel(char mode){
    int sub_choice;
    while(1){
        fetch_index();
        printf("Hello %s,please select subject\n0)Back\n1)Physics\n2)Maths\n3)C programing\n4)Linux\n5)Problem solving\n->",student_details.name);
        sub_choice = safe_num_extract(5);
        switch (sub_choice){
            case 1:
                strcpy(quizz_details.subject,"physics");
                ndx.global_tmp=ndx.physics;
                database(mode, &quizz_details);
            break;
            case 2:
                strcpy(quizz_details.subject,"maths");
                ndx.global_tmp=ndx.maths;
                database(mode, &quizz_details);
            break;
            case 3:
                strcpy(quizz_details.subject,"c_prog");
                ndx.global_tmp=ndx.c_prog;
                database(mode, &quizz_details);
            break;
            case 4:
                strcpy(quizz_details.subject,"linux");
                ndx.global_tmp=ndx.linux;
                database(mode, &quizz_details);
            break;
            case 5:
                strcpy(quizz_details.subject,"prob_solving");
                ndx.global_tmp=ndx.prob;
                database(mode, &quizz_details);
            break;
            case 0:
                system("cls");
                return 1;
            break;
        default:
            printf("Invalid choice selection\n");
            pretty_little_loading_bar();
            stu_subject_panel(mode);
        break;        
        }
    } 
}
int quizz_take(){
    get_number_of_questions();
    int i=1;
    char tmp_marks_string[10],response_filename[200],response_file_path_BUFFER[200];
    //strcpy(student_details.SAP_id,"590021474"); BATCH LOGIC DEBUG LINE
    FILE *quizz_file = fopen(quizz_details.quizz_filename,"r");
    if(quizz_file == NULL){
        red();
        printf("\nERROR! file 'quizz' failed to initialise\n");
        reset();
        press_enter();
        exit(0);
    }
    
    fgets(quizz_details.quizz_name, sizeof(quizz_details.quizz_name), quizz_file);
    quizz_details.quizz_name[strcspn(quizz_details.quizz_name,"\n")] = '\0';
    strcpy(response_file_path_BUFFER,quizz_details.quizz_name);
    
    for (int i = 0; response_file_path_BUFFER[i] != '\0'; i++){
        if(response_file_path_BUFFER[i] == ' '){
            response_file_path_BUFFER[i] = '_';
        }
    }
    sprintf(response_filename,"responses/%s/B7/%s/%s.txt",quizz_details.subject,response_file_path_BUFFER,student_details.SAP_id);
    FILE *responses = fopen(response_filename,"w");
    fgets(tmp_marks_string,sizeof(tmp_marks_string), quizz_file);
    tmp_marks_string[strcspn(tmp_marks_string,"\n")] = '\0';
    sscanf(tmp_marks_string,"%d %d", &stu_quizz.positive_marks, &stu_quizz.negative_marks);
    fprintf(responses,"%d %d %d\n%s\n%s\n%s\n",stu_quizz.positive_marks,stu_quizz.negative_marks,quizz_details.number_of_questions, quizz_details.subject,student_details.SAP_id, quizz_details.quizz_name);
    while(i <= quizz_details.number_of_questions){
        fgets(stu_quizz.question, sizeof(stu_quizz.question), quizz_file);
        stu_quizz.question[strcspn(stu_quizz.question,"\n")] = '\0';
        fgets(stu_quizz.optionA, sizeof(stu_quizz.optionA), quizz_file);
        stu_quizz.optionA[strcspn(stu_quizz.optionA,"\n")] = '\0';
        fgets(stu_quizz.optionB, sizeof(stu_quizz.optionB), quizz_file);
        stu_quizz.optionB[strcspn(stu_quizz.optionB,"\n")] = '\0';
        fgets(stu_quizz.optionC, sizeof(stu_quizz.optionC), quizz_file);
        stu_quizz.optionC[strcspn(stu_quizz.optionC,"\n")] = '\0';
        fgets(stu_quizz.optionD, sizeof(stu_quizz.optionD), quizz_file);
        stu_quizz.optionD[strcspn(stu_quizz.optionD,"\n")] = '\0';
        printf("%s\n%s\n%s\n%s\n%s\n->",stu_quizz.question,stu_quizz.optionA,stu_quizz.optionB,stu_quizz.optionC,stu_quizz.optionD);
        stu_quizz.picked_option = safe_option_extract();
        printf("\n");
        fprintf(responses,"Question_%d:%c\n",i,stu_quizz.picked_option);
        i++;
    }
    printf("Thank you quizz has been submitted for evaluation!\n");
    green();
    press_enter();
    reset();
    system("cls");
    fclose(quizz_file);
    fclose(responses);
    evaluator();
    return 1;
}
int evaluator(){
    fetch_index();
    int i=1,total_score = 0;
    char records_folder_path[200],record_file_path[200], response_filename[200], response_file_path_BUFFER[200],answer_file_path[200],corect_option,selected_option,line[200], kudda[100];
    //int correct_marks,wrong_marks; LOCAL VARIABLES no global
    strcpy(response_file_path_BUFFER,quizz_details.quizz_name);
    for(int i = 0; response_file_path_BUFFER[i] != '\0'; i++){
        if(response_file_path_BUFFER[i] == ' '){
            response_file_path_BUFFER[i] = '_';
        }
    }
    sprintf(response_filename,"responses/%s/B7/%s/%s.txt", quizz_details.subject,response_file_path_BUFFER,student_details.SAP_id);
    sprintf(answer_file_path,"quizzes/%s/answers/%s_%d.txt",quizz_details.subject,response_file_path_BUFFER, ndx.global_tmp);
    FILE *responses = fopen(response_filename,"r");
    FILE *answers = fopen(answer_file_path,"r");
    sprintf(records_folder_path,"analytics/%s/B7/%s",quizz_details.subject,student_details.SAP_id);
    if(folder_exists(records_folder_path) == 0){
        CreateDirectory(records_folder_path, NULL);
    }
    sprintf(record_file_path,"analytics/%s/B7/%s/%s_record_%d.txt",quizz_details.subject,student_details.SAP_id,quizz_details.subject,ndx.global_tmp);
    
    //USING tmp variables from analytics structure-> TO BE REPLACED WITH SHORTER REFFERENCE STRUCTS!
    analytics_tmp.correct_QN_index = 0;
    analytics_tmp.incorrect_QN_index = 0;
    analytics_tmp.skipped_QN_index = 0; 
    FILE *records = fopen(record_file_path,"w");
    fprintf(records,"%s\n", quizz_details.quizz_name);
    fgets(line,sizeof(line),responses);
    line[strcspn(line,"\n")] = '\0';
    sscanf(line,"%d %d %d", &stu_quizz.positive_marks, &stu_quizz.negative_marks, &quizz_details.number_of_questions);
    int max_marks = quizz_details.number_of_questions*stu_quizz.positive_marks;
    fgets(line,sizeof(line), responses);
    line[strcspn(line,"\n")] = '\0';
    sscanf(line,"%s", quizz_details.subject);
    fgets(line,sizeof(line), responses);
    line[strcspn(line,"\n")] = '\0';
    sscanf(line, "%s", kudda);
    fgets(line,sizeof(line), responses);
    line[strcspn(line,"\n")] = '\0';
    sscanf(line,"%s", quizz_details.quizz_name);
    while(i <= quizz_details.number_of_questions){
        fgets(line, sizeof(line), responses);
        sscanf(line, "Question_%*d:%c", &selected_option);
        fgets(line, sizeof(line), answers);
        sscanf(line, "Question_%*d:%c", &corect_option);
        if (selected_option == 's'){
            i++;
            continue;
        }
        if(selected_option == corect_option){
            total_score = total_score + stu_quizz.positive_marks;
            analytics_tmp.correct_question_numbers[analytics_tmp.correct_QN_index] = i;
            analytics_tmp.correct_QN_index++;

        }else{
            total_score = total_score + stu_quizz.negative_marks;
            analytics_tmp.incorrect_question_numbers[analytics_tmp.incorrect_QN_index] = i;
            analytics_tmp.incorrect_QN_index++;
        }
        i++;
    }
    fprintf(records, "%d/%d\n", total_score, max_marks);
    if(analytics_tmp.correct_QN_index != 0){
        for(int i = 0; i < analytics_tmp.correct_QN_index; i++){
            fprintf(records," %d", analytics_tmp.correct_question_numbers[i]);
        }
        fprintf(records, "\n");
    }else{
        fprintf(records,"0\n");
    }
    if(analytics_tmp.incorrect_QN_index != 0){
        for(int i = 0; i < analytics_tmp.incorrect_QN_index; i++){
            fprintf(records," %d", analytics_tmp.incorrect_question_numbers[i]);
        }
    }
    fclose(responses);
    fclose(answers);
    fclose(records);
    return (total_score);
    
}

//DATABASE
int user_database(){
    char user_details_buffer[100], user_details[100];
    char username[100], username_setup_check[100], name[100], name_[100], name__[100], sap_id[100], buffer[100], batch[2];
    int choice, sanity = 0, row_trk;
    system("cls");
    hide_cursor();
    red();
    scary_little_loading_bar(19,1, '.');
    printf("\nUSER CONTROL PANNEL\n");
    scary_little_loading_bar(19,1, '.');
    reset();
    // yellow();
    // printf("\n0)Exit\n1)List all users\n2)Create new user\n->");
    // reset();
    //choice = safe_num_extract(2);
    row_trk = 4;
    gotoXY(row_trk, 1);
    red();
    printf("->");
    reset();
    while(sanity != 1){

        print_menu_row("List All Users", 4, 3, row_trk == 4, "blue");
        print_menu_row("Create new user", 5, 3, row_trk == 5, "blue");
        print_menu_row("EXIT", 6, 3, row_trk == 6, "red");

        menu_fix_logic(&choice, &row_trk, &sanity, 6, 4, 1);
    }show_cursor();
    
    switch(choice){
        case 1:
            system("cls");
             //tabble data
            int name_length, username_length, serial_num_trk = 1, space_pad=0; //gonna need to track serial number length but only when list has more than 9 entries                             DORMANT
            FILE *cred = fopen("credentials/user.txt","r");
            hide_cursor();
            red();
            //scary_little_loading_bar(79, 1, '=');
            fast_little_loading_bar(79, '=');
            Sleep(100); //Dramatic effect
            printf("\n||  No.  ||            Name            ||       User Name       ||   SAP ID  ||\n");
            Sleep(100);
            fast_little_loading_bar(79, '=');
            //scary_little_loading_bar(79, 1, '=');
            reset();
            blue();
            while(fgets(user_details_buffer, sizeof(user_details_buffer), cred) != NULL){
                user_details_buffer[strcspn(user_details_buffer,"\n")] = '\0';
                sscanf(user_details_buffer, "%s %s %s %s %s", username, buffer, name__, sap_id, batch);
                strcpy(username_setup_check, username);
                replace_underscore(name__, name);
                name_length = strlen(name);
                username_length = strlen(username);
                
                //Dynamically constructing name to fit 28 spaces
                for(int i = name_length; i < 28; i++){
                    strcpy(buffer, name);
                    sprintf(name, "%s ", buffer);
                    //printf("%d ", i); debug
                }
                //Dynamically constructing username to fit 23 spaces
                for(int i = username_length; i < 23; i++){
                    strcpy(buffer, username);
                    sprintf(username, "%s ", buffer);
                    //printf("%d ", i); debug
                }
                Sleep(50);
                //If user has not setup account
                if(strcmp(username_setup_check, "setup") == 0){
                strcpy(username_setup_check, "SETUP NOT COMPLETE");
                username_length = strlen(username_setup_check);
                for(int i = username_length; i < 23; i++){
                    strcpy(buffer, username_setup_check);
                    sprintf(username_setup_check, "%s ", buffer);
                    //printf("%d ", i); debug
                }
                blue();
                printf("\n||   %d   ||%s||", serial_num_trk, name);
                yellow();
                printf("%s", username_setup_check);
                blue();
                printf("|| %s ||", sap_id);
                reset();
                serial_num_trk++;
                continue;
                }
                //Regular printing
                printf("\n||   %d   ||%s||%s|| %s ||", serial_num_trk, name, username, sap_id);
                serial_num_trk++;
            }
            fclose(cred);
            red();
            printf("\n");
            scary_little_loading_bar(79, 3, '=');
            reset();
            show_cursor();
            press_enter();
            system("cls");
            user_database();
        break;

        case 2:
            system("cls");
            FILE *users = fopen("credentials/user.txt","a");
            red();
            gotoXY(1,1);
            blue();
            scary_little_loading_bar(30,0,'=');
            red();
            printf("\n||NAME-> \n||SAP ID-> \n||BATCH-> ");
            reset();
            gated_string_input(name, 20, 1, 2, 10, 0, 0, 1, 1);
            replace_space(name, name_);
            gated_string_input(sap_id, 9, 1, 3, 12, 0, 0, 0, 1);
            gated_string_input(batch, 2, 1, 4, 11, 0, 0, 0, 1);
            fprintf(users, "\nsetup setup %s %s %s", name_, sap_id, batch);
            blue();
            scary_little_loading_bar(30, 0, '=');
            green();
            printf("\nUser created succesfully\n");
            reset();
            press_enter();
            fclose(users);
            system("cls");
            user_database();
        break;
    }
}
int database(char identity, struct quiz_details *qd){
    int i=1,quizz_selection,indexx=ndx.global_tmp;
    char tmp_name[100],quizz_filename[100], Garbage[100];
    //printf("0)Back");
    switch (identity){
        case 'q':
            //Printing list of quizzes
            i = 1;
            system("cls");
            yellow();
            printf("0)Back");
            reset();
            while(i <= indexx){
                sprintf(quizz_filename,"quizzes/%s/%s_quizz_%d.txt", qd->subject, qd->subject, i);
                FILE *quizz = fopen(quizz_filename,"r");
                if(quizz == NULL){
                    red();
                    printf("ERROR! Missing or corupt quizz file\n");
                    press_enter();
                    reset();
                    i++;
                }else{
                fgets(tmp_name, sizeof(tmp_name),quizz);
                tmp_name[strcspn(tmp_name,"\n")] = '\0';
                printf("\n%d)%s",i,tmp_name);
                fclose(quizz);
                i++;
                }
            }
            
            //Quizz selection altering global struct variables to pass to quizz_take() funtion
            blue();
            printf("\nWhich quizz would you like to attempt?\n->");
            reset();
            quizz_selection = safe_num_extract(indexx);
            if(quizz_selection == 0){
                system("cls");
                return 1;
            }
            sprintf(quizz_filename,"quizzes/%s/%s_quizz_%d.txt",qd->subject ,qd->subject, quizz_selection);
            strcpy(qd->quizz_filename, quizz_filename);
            yellow();
            printf("\nInitialising");
            pretty_little_loading_bar();
            reset();
            system("cls");
            ndx.global_tmp=quizz_selection;
            quizz_take();
        break;
        
        
        //PERFORMANCE PORTAL
        case 'p':{
            char analytics_filename[200],answer_file_path[100],correct_option_BUFFER[100], response_option_BUFFER[100];
            int detailed;
            i = 1;
            system("cls");
            //Printing list of analytics files
            yellow();
            printf("0)Back");
            reset();
            while(i <= indexx){
                sprintf(tmp_name,"analytics/%s/B7/%s/%s_record_%d.txt",qd->subject, student_details.SAP_id, qd->subject, i);
                FILE *records = fopen(tmp_name,"r");
                if(records == NULL){
                    break;
                    // red();
                    // printf("\nERROR! Missing or corupt quizz file\n");
                    // reset();
                    // press_enter();
                    //i++;
                }else{
                    fgets(tmp_name, sizeof(tmp_name),records);
                    tmp_name[strcspn(tmp_name,"\n")] = '\0';
                    printf("\n%d)%s",i,tmp_name);
                    fclose(records);
                i++;
                }
            }
            
            //Analytics file selection
            blue();
            printf("\nEnter which quiz to view analytics: ");
            reset();
            quizz_selection = safe_num_extract(i);
            //scanf("%d",&quizz_selection); finally no more buffer overflow!
            sprintf(quizz_filename,"quizzes/%s/%s_quizz_%d.txt",qd->subject, qd->subject, quizz_selection);
            strcpy(qd->quizz_filename,quizz_filename);
            if(quizz_selection == 0){
                system("cls");
                return 1;
            }
            sprintf(analytics_filename,"analytics/%s/B7/%s/%s_record_%d.txt", qd->subject, student_details.SAP_id, qd->subject, quizz_selection); 
            
            //OPPENING ANALYTICS FILE
            FILE *analytics = fopen(analytics_filename,"r");
            if (analytics == NULL){
                red();
                printf("\nERROR! Analytics file failed to load\n");
                reset();
                press_enter();
                return 1;
            }
            
            fgets(Garbage,sizeof(Garbage), analytics);
            Garbage[strcspn(Garbage,"\n")] = '\0';

            fgets(Garbage,sizeof(Garbage), analytics);
            Garbage[strcspn(Garbage,"\n")] = '\0';
            sscanf(Garbage, "%d/%d", &student_details.marks, &student_details.maximum_marks);
            
            //Printing marks and asking user if they want to view detailed analytics
            printf("Quizz name: %s\nMarks recieved-> %d\nMax Marks-> %d\n", qd->quizz_name, student_details.marks, student_details.maximum_marks);
            yellow();
            printf("\nDo you want to view detailed analytics?\n1)Yes\n");
            reset();
            red();
            printf("2)No\n");
            reset();
            printf("->");
            detailed = safe_num_extract(2);
            
            switch(detailed){
                case 1:
                    system("cls");
                    char response_filename[100], quizz_name_underscores[100];
                    int garbage, garbage1, details_selection;
                    
                    FILE *qz = fopen(quizz_filename,"r");
                    //REPLACING SPACE WITH '_'
                    fgets(qd->quizz_name, sizeof(qd->quizz_name), qz);
                    qd->quizz_name[strcspn(qd->quizz_name,"\n")] = '\0';
                    strcpy(quizz_name_underscores, qd->quizz_name);
                    for(int i = 0; quizz_name_underscores[i] != '\0'; i++){
                        if(quizz_name_underscores[i] == ' '){
                            quizz_name_underscores[i] = '_';
                        }
                    }
                    
                    //OPENING RESPONSE FILE and skipping 4 lines via 'for' loop
                    sprintf(response_filename, "responses/%s/B7/%s/%s.txt", qd->subject, quizz_name_underscores, student_details.SAP_id);
                    FILE *response = fopen(response_filename,"r");
                    if(response == NULL){
                        red();
                        printf("ERROR! File 'response.txt' failed to initialise.");
                        press_enter();
                        reset();
                        return 1;
                    }
                    for (int i = 0; i<=3;i++){
                        fgets(Garbage,sizeof(Garbage), response);
                        Garbage[strcspn(Garbage,"\n")] = '\0';
                    }
                    
                    //OPENING ANSWER FILE
                    sprintf(answer_file_path,"quizzes/%s/answers/%s_%d.txt", qd->subject, quizz_name_underscores, quizz_selection);
                    FILE *ans = fopen(answer_file_path,"r");
                    if(ans == NULL){
                        red();
                        printf("ERROR! File 'answer' failed to initialise\n");
                        press_enter();
                        reset();
                        return 1;
                    }
                    
                    printf("Analytics Menu\n0)Back\n1)View response sheet\n2)List of correct questions\n3)List of incorrect question\n->");
                    //while(getchar() != '\n'); i hate this language
                    details_selection = safe_num_extract(3);
                    
                    if(details_selection == 0){
                        //Gonna do via recursions
                        WIP();
                    }else if(details_selection == 1){
                        //SKIPPING THE MARKS LINE FROM QUIZZ FILE for now                                                                                                                       EXTRASS
                        get_number_of_questions();
                        fgets(Garbage,sizeof(Garbage), qz);
                        system("cls");
                            
                        for(int i = 1; i <= qd->number_of_questions ; i++){
                            //Extracting question
                            fgets(analytics_tmp.question_tmp,sizeof(analytics_tmp.question_tmp), qz);
                            analytics_tmp.question_tmp[strcspn(analytics_tmp.question_tmp,"\n")] = '\0';
                            //Extracting option a
                            fgets(analytics_tmp.tmp_optA,sizeof(analytics_tmp.tmp_optA), qz);
                            analytics_tmp.tmp_optA[strcspn(analytics_tmp.tmp_optA, "\n")] = '\0';
                            //Extracting option b
                            fgets(analytics_tmp.tmp_optB,sizeof(analytics_tmp.tmp_optB), qz);
                            analytics_tmp.tmp_optB[strcspn(analytics_tmp.tmp_optB, "\n")] = '\0';
                            //Extracting option c
                            fgets(analytics_tmp.tmp_optC,sizeof(analytics_tmp.tmp_optC), qz);
                            analytics_tmp.tmp_optC[strcspn(analytics_tmp.tmp_optC, "\n")] = '\0';
                            //Extracting option d
                            fgets(analytics_tmp.tmp_optD,sizeof(analytics_tmp.tmp_optD), qz);
                            analytics_tmp.tmp_optD[strcspn(analytics_tmp.tmp_optD, "\n")] = '\0';
                            //Extracting correct option 
                            fgets(correct_option_BUFFER,sizeof(correct_option_BUFFER), ans);
                            correct_option_BUFFER[strcspn(correct_option_BUFFER,"\n")] = '\0';
                            sscanf(correct_option_BUFFER,"Question_%d:%c", &garbage, &analytics_tmp.correct_option);
                            //Extracting option picked by user
                            fgets(response_option_BUFFER, sizeof(response_option_BUFFER), response);
                            response_option_BUFFER[strcspn(response_option_BUFFER,"\n")] = '\0';
                            sscanf(response_option_BUFFER,"Question_%d:%c", &garbage1, &analytics_tmp.response);
                            
                            //Printing everything
                            printf("%s\n%s\n%s\n%s\n%s\nCorrect Option: %c\n", analytics_tmp.question_tmp, analytics_tmp.tmp_optA, analytics_tmp.tmp_optB, analytics_tmp.tmp_optC, analytics_tmp.tmp_optD, analytics_tmp.correct_option);
                            //Printing color coded options
                            if(analytics_tmp.response == 's'){
                                yellow();
                                printf("Response: Skipped\n\n");
                                reset();
                            }else if(analytics_tmp.response == analytics_tmp.correct_option){
                                green();
                                printf("Response: %c\n\n", analytics_tmp.response);
                                reset();
                            }else{
                                red();
                                printf("Response: %c\n\n", analytics_tmp.response);
                                reset();
                            }
                        }
                        //Closing all files
                        fclose(qz);
                        fclose(response);
                        fclose(ans);
                        fclose(analytics);
                        green();
                        press_enter();
                        reset();
                        system("cls");
                        return 1;
                    }else if(details_selection == 2){
                        WIP();
                    }else if(details_selection == 3){
                        WIP();
                    }


            }
            green();
            press_enter();
            reset();
            fclose(analytics);
            system("cls");
        break;
        }
        case 'A':
            printf("Which quizz would you like to modify?\n->");
            scanf("%d",&quizz_selection);
            if(quizz_selection == 0){
                system("cls");
                return 1;
            }
            sprintf(quizz_filename,"quizzes/%s/%s_quizz_%d.txt",quizz_details.subject,quizz_details.subject,quizz_selection);
        break;
        case 'a':
            printf("Which quizz would you like to view?\n->");
            scanf("%d",&quizz_selection);
            if(quizz_selection == 0){
                system("cls");
                return 1;
            }
            sprintf(quizz_filename,"quizzes/%s/%s_quizz_%d.txt",quizz_details.subject,quizz_details.subject,quizz_selection);
        break;
    }
    return 1; 
}
int performance_analytics(char mode){
WIP();
}

//ADMIN PANEL
int admin_check(){
    char f_usr[20], usr[20],f_pass[20], pass[20],tmp;
    int i=0;
    FILE *credentials = fopen("credentials/admin.txt", "r"); 
   if (credentials == NULL) {
        red();
        printf("\nFATAL ERROR! Critical credential file failed to initialise\n");
        reset();
        press_enter();
        exit(0); //Should exit as credential is a required file for code to funciton
    }
    fscanf(credentials, "%s %s", f_usr, f_pass);
    
    printf("Please enter username: ");
    fgets(usr, sizeof(usr), stdin);
    usr[strcspn(usr, "\n")] = '\0';

    printf("Please enter password: ");
    while(1){
        tmp = getch();
        if(tmp == '\r'){
            break;
        }
        if(tmp == '\b' && i > 0){
            i--;
            pass[i] = '\0';
            printf("\b \b");
            continue;
        }
        printf("*");
        pass[i++] = tmp;
    }pass[i] = '\0';
    //printf("%s", pass); Debug line
    if(strcmp(f_usr,usr)==0 && strcmp(f_pass,pass)==0){
        fclose(credentials);
        return 1;
    }else{
        fclose(credentials);
        return 2;
    }
}
void admin_mode(){
    char name[20],usr[20],pas[20];
    FILE *cred = fopen("credentials/admin.txt","r");
    if(cred == NULL){
        red();
        printf("ERROR! Critical credential file failed to initialise");
        reset();
        exit(0);
    }
    fscanf(cred,"%s %s %s",usr,pas,name);
    fclose(cred);
    
while(1){
    system("cls");
    int in, sanity = 0, row_trk=2;
    yellow();
    printf("Welcome %s, Please select funtion:", name);
    hide_cursor();
    gotoXY(2,1);
    red();
    printf("->");
    reset();
    while(sanity != 1){
        print_menu_row("Quizzes", 2 , 3, row_trk == 2, "red");
        print_menu_row("Performance Analytics", 3 , 3, row_trk == 3, "red");
        print_menu_row("User Database Management", 4 , 3, row_trk == 4, "red");
        print_menu_row("EXIT", 5 , 3, row_trk == 5, "red");
        
        menu_fix_logic(&in, &row_trk, &sanity, 5, 2, 1);
    
    }show_cursor();
    
    
    //in = safe_num_extract(3);
    if (in == 1){
        yellow();
        printf("\nLoading quiz board");
        pretty_little_loading_bar();
        reset();
        system("cls");
        int quiz_choice;
        printf("Please select funtion:\n0)Main menu\n1)Make quiz\n2)Edit existing quizes\n->");
        quiz_choice = safe_num_extract(3);
        if (quiz_choice == 1){
            make_quiz();
        }else if(quiz_choice == 2){
            //edit_quiz();
        }else if(quiz_choice == 0){
            system("cls");
            continue;
        }

    }else if(in == 2){
        //pfr_analytics(); May not use and use database()
        WIP();
    }else if(in == 3){
        user_database();

    }else if(in == 4){
        yellow();
        printf("\nLogging out");
        pretty_little_loading_bar();
        reset();
        return;
    }
}
}
int make_quiz(){
    fetch_index();
    int qz_sub,qz_type;
    char subject[20],type[20],file_name[100];
    printf("\nPlease select subject:\n0)Back\n1)Physics\n2)Maths\n3)C programming\n4)Linux\n5)Problem solving\n->");
    qz_sub = safe_num_extract(5);

    switch (qz_sub){
        case 1:
            strcpy(quizz_details.subject,"physics");
            ndx.global_tmp = ndx.physics;
            sprintf(file_name,"quizzes/physics/physics_quizz_%d.txt",ndx.physics+1);
        break;
        case 2:
            strcpy(quizz_details.subject,"maths");
            ndx.global_tmp = ndx.maths;
            sprintf(file_name,"quizzes/maths/maths_quizz_%d.txt",ndx.maths+1);
        break;
        case 3:
            strcpy(quizz_details.subject,"c_prog");
            ndx.global_tmp = ndx.c_prog;
            sprintf(file_name,"quizzes/c_prog/c_prog_quizz_%d.txt",ndx.c_prog+1);
        break;
        case 4:
            strcpy(quizz_details.subject,"linux");
            ndx.global_tmp = ndx.linux;
            sprintf(file_name,"quizzes/linux/linux_quizz_%d.txt",ndx.linux+1);
        break;
        case 5:
            strcpy(quizz_details.subject,"prob_solving");
            ndx.global_tmp = ndx.prob;
            sprintf(file_name,"quizzes/prob_solving/prob_solving_quizz_%d.txt",ndx.prob+1);
        break;
        case 0:
            system("cls");
            return 1;
    }
    strcpy(quizz_details.quizz_filename,file_name);   
    printf("\nPlease select quiz type:\n1)MCQ\n2)Fill in the blanks\n->");
    qz_type = safe_num_extract(2);
    // switch (qz_type){ (OLD Secondary aproach to flag quizz type) 
    //     case 1:
    //         strcpy(type,"MCQ");
    //     break;                           
    //     case 2:
    //         //strcpy(type,"Fill in the blanks");
    //         WIP();
    //     break;
    // }
    
    if(qz_type == 1){
        printf("Loading");
        pretty_little_loading_bar();
        system("cls");
        mcq_make();
        return 1;
    }else if(qz_type == 2){
        //FITB();
        WIP();
    }
}
int mcq_make(){
    
    int marks_correct, marks_incorrect,i=1;
    char answer_file_path[200],answer_file_path_BUFFER[200], response_folder_allocation[200], response_folder_allocation_BUFFER[200],tmp_question[100],tmp_optA[50],tmp_optB[50],tmp_optC[50],tmp_optD[50],correct_option;
    //QUIZ FILE MAKING
    FILE *MCQ = fopen(quizz_details.quizz_filename,"w");
    if(MCQ == NULL){
        printf("ERROR! Quizz file not initialised\n");
        press_enter();
        return 1;
    }
    //while (getchar() != '\n');
    printf("Please enter title of quizz: ");
    fgets(quizz_details.quizz_name,sizeof(quizz_details.quizz_name),stdin);
    quizz_details.quizz_name[strcspn(quizz_details.quizz_name,"\n")] = '\0';
    //RESPONSE FOLDER GENERATION
    strcpy(response_folder_allocation_BUFFER,quizz_details.quizz_name);
    for (int i = 0; response_folder_allocation_BUFFER[i] != '\0'; i++){
        if(response_folder_allocation_BUFFER[i] == ' '){
            response_folder_allocation_BUFFER[i] = '_';
        }
    }
    sprintf(response_folder_allocation,"responses/%s/B7/%s",quizz_details.subject,response_folder_allocation_BUFFER);
    CreateDirectory(response_folder_allocation, NULL);
    //ANSWER FILE LOADING
    strcpy(answer_file_path_BUFFER, response_folder_allocation_BUFFER);
    sprintf(answer_file_path,"quizzes/%s/answers/%s_%d.txt",quizz_details.subject,answer_file_path_BUFFER,ndx.global_tmp+1);
    FILE *answers = fopen(answer_file_path,"w");
    printf("Please enter marking scheme:\nCorrect: ");
    scanf("%d", &marks_correct);
    printf("Incorrect: ");
    scanf("%d", &marks_incorrect);
    fprintf(MCQ, "%s\n%d %d\n", quizz_details.quizz_name, marks_correct, marks_incorrect);
    getchar();
    
    while(1){
    
        printf("\nEnter question %d or type EXIT to stop: ", i);
        fgets(tmp_question, sizeof(tmp_question), stdin);
        tmp_question[strcspn(tmp_question, "\n")] = '\0';

        if(strcmp(tmp_question,"EXIT")==0){
            break;
        }
        printf("Option A: ");
        fgets(tmp_optA, sizeof(tmp_optA), stdin);
        tmp_optA[strcspn(tmp_optA, "\n")] = '\0';
        printf("Option B: ");
        fgets(tmp_optB, sizeof(tmp_optB), stdin);
        tmp_optB[strcspn(tmp_optB, "\n")] = '\0';
        printf("Option C: ");
        fgets(tmp_optC, sizeof(tmp_optC), stdin);
        tmp_optC[strcspn(tmp_optC, "\n")] = '\0';
        printf("Option D: ");
        fgets(tmp_optD, sizeof(tmp_optD), stdin);
        tmp_optD[strcspn(tmp_optD, "\n")] = '\0';
        printf("Correct option(lower case): ");
        correct_option = safe_option_extract();
        //while(getchar() != '\n');
        fprintf(MCQ, "Question %d:%s\nA:%s\nB:%s\nC:%s\nD:%s\n", i,tmp_question,tmp_optA,tmp_optB,tmp_optC,tmp_optD);
        fprintf(answers,"Question_%d:%c\n",i,correct_option);
        i++;
        Sleep(100);
    }
    printf("%s quiz has been saved succesfully\n",quizz_details.subject);
    fclose(MCQ);
    fclose(answers);
    append_index(quizz_details.subject);
    FILE *mcq = fopen(quizz_details.quizz_filename,"a");
    fprintf(mcq,"TOTAL_QUESTIONS %d",i-1);
    fclose(mcq);
    //get_number_of_questions(); legacy line
    pretty_little_loading_bar();
}