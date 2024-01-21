
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*********Déclaration des variables globales*********/
char symbole[10]; //Stocke le symbole actuel
char caractere=' ';
int id;
char *integer="i";
FILE* f; //Pointeur vers le fichier source
FILE* f_i; //Pointeurs vers le fichier de sortie du code intermédiaire
int x=0;
char *ch="c";
char *bool="bool";
const char* mots_cles[] = {"program", "var", "integer", "char", "begin", "end", "if", "then", "else", "while", "do", "read", "readln", "write", "writeln"};
int etiquette=0;

// la structure d'un symbole
struct SymboleStruct {
    int id;
    char* symbole;
    char type;
};
// Table des symboles
struct SymboleStruct *table_symbole=NULL;
int taille_table_symbole=0;


int *tab_identificateurs[10];
int taille_tab_identificateurs=0;
// Les fonctions disponnibles
void P();
void Dcl();
void A_prime();
void Liste_id();
void B_prime();
void Type();
void Inst_composee();
void Inst();
void Liste_inst();
void K();
void I();
void Exp();
void C_prime();
void Exp_simple();
void D_prime();
void Terme();
void E_prime();
void Facteur();


// Fonction pour obtenir le caractère suivant à partir du fichier
char caractereSuivant(FILE* f){
    return fgetc(f);
}
//Fct RangerId qui renvoie 1 si le string est un mot clé , 0 sinon
int RangerId(const char* chaine, const char* mots_cles[]) {
    for (int i = 0; i < 15; ++i) {
        if (strcmp(chaine, mots_cles[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

//ajouterSymbole
//Ajoute un caractère à une ch
char* ajouterSymbole(char* ch, char caractere) {
    int len = strlen(ch);
    char* chaine = (char*)malloc((len + 2) * sizeof(char));

    strcpy(chaine, ch);
    chaine[len] = caractere;
    chaine[len + 1] = '\0';

    return chaine;
}


/*********AnalyseurLexical*********/
//Parcours le codeS c par c et identifie les tokens (Unité lexicaux)
void AnalyseurLexical(){
    int condition = 0;
    int etat = 0;
    char ch[10]="";
    while(condition==0){
        if(x==0){
            caractere=caractereSuivant(f);
        }
        x=0;
        switch(etat){
        case 0:
            if(caractere==' ' || caractere=='\t' || caractere=='\n'){
                etat=0;
            }
            else if(isalpha(caractere)){
                etat=1;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }
            else if(isdigit(caractere)){
                etat=3;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }
            else if(caractere=='+'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opadd");
                condition=1;
            }
            else if(caractere=='-'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opadd");
                condition=1;
            }
            else if(caractere=='|'){etat=7;}
            else if(caractere=='='){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "oprel");
                condition=1;
            }
            else if(caractere=='<'){
                etat=10;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }
            else if(caractere=='>'){
                etat=14;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }
            else if(caractere=='*'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opmul");
                condition=1;
            }
            else if(caractere=='/'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opmul");
                condition=1;
            }
            else if(caractere=='%'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opmul");
                condition=1;
            }
            else if(caractere=='&'){etat=5;}
            else if(caractere==':'){
                etat=22;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }
            else if(caractere==';'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, ch);
                condition=1;
            }
            else if(caractere==','){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, ch);
                condition=1;
            }
            else if(caractere=='('){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, ch);
                condition=1;
            }
            else if(caractere==')'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, ch);
                condition=1;
            }
            break;
        case 1:
            if(isalpha(caractere) || isdigit(caractere)){
                etat=1;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }else{
                if(RangerId(ch,  mots_cles)){
                    strcpy(symbole, ch);
                }else{
                    strcpy(symbole, "id");
                }
                x=1;
                condition=1;
            }
            break;

        case 3:
            if(isdigit(caractere)){
                etat=3;
                strcpy(ch, ajouterSymbole(ch, caractere));
            }else{
                strcpy(symbole, "nb");
                x=1;
                condition=1;
            }
            break;
        case 5:
            if(caractere=='&'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opmul");
                condition=1;
            }else{
                strcpy(symbole, "erreur");
                condition=1;
            }
            break;
        case 7:
            if(caractere=='|'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "opadd");
                condition=1;
            }else{
                strcpy(symbole, "erreur");
                condition=1;
            }
            break;
        case 10:
            if(caractere=='>'){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "oprel");
                condition=1;
            }else if(caractere=='='){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "oprel");
                condition=1;
            }
            else{
                strcpy(symbole, "oprel");
                x=1;
                condition=1;
            }
            break;
        case 14:
            if(caractere=='='){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, "oprel");
                condition=1;
            }else{
                strcpy(symbole, "oprel");
                x=1;
                condition=1;
            }
            break;
        case 22:
            if(caractere=='='){
                strcpy(ch, ajouterSymbole(ch, caractere));
                strcpy(symbole, ch);
                condition=1;
            }else{
                strcpy(symbole, ch);
                x=1;
                condition=1;
            }
            break;
        }

        if (caractere == EOF) {
            condition = 1;
        }
    }
}

// procédure de validation du symbole
void accepter(char *t) {
    if (strcmp(symbole, t) == 0) {
        AnalyseurLexical();
    } else {
        erreur();
        printf("il faut faire entrer '%s' \n",t);
        exit(0);

    }
}

// procédure d'erreur (Appelée en cas d'erreur)
void erreur() {
    printf("Erreur\n");
}






// Implémentation des procédures des règles de production
void P() {
	accepter("program");
	accepter("id");
	accepter(";");
	Dcl();
	Inst_composee();
    if (strcmp(symbole,".")==0){
	    printf("Correct et coherent)");
	   }
	else{
        erreur();
		exit(0);
    }
}
void Dcl() {
    A_prime();
}

void A_prime() {
	if(strcmp(symbole,"var")==0)
	{
        for (int i = 0; i < 10; ++i) {
            tab_identificateurs[i] = NULL;
        }
        taille_tab_identificateurs=0;
		accepter("var");
		Liste_id();
		accepter(":");
		Type();
		accepter(";");
		A_prime();
	}
}

void Liste_id() {
    if (strcmp(symbole,"id")==0){
	{
		accepter("id");
		B_prime();
	}
    }
	else
	{
        erreur();
	    exit(0);
}

}

void B_prime(){
	if (strcmp(symbole,",")==0)
	{
		accepter(",");
		accepter("id");
		B_prime();
	}

}

void Type() {
    if (strcmp(symbole,"integer")==0){
        accepter("integer");
        ajouter_plusieurs('i');
    }
	else if (strcmp(symbole,"char")==0){
	    accepter("char");
        ajouter_plusieurs('c');
	}
	else
	{
	printf(" '%s' n'est pas un type valide \n", symbole);
	exit(0);

	}
}

void Inst_composee() {
	accepter("begin");
	Inst();
	accepter("end");
	}


void Inst() {
    Liste_inst();
}

void Liste_inst() {
		I();
		K();

}

void K() {
    if (strcmp(symbole,";")==0){
        accepter(";");
        I();
        K();
    }

}

void I() {
    char type1;
    char type2;
    if (strcmp(symbole,"id")==0){
        accepter("id");
        emettre2("valeurg",table_symbole[id].symbole);
        type1=chercherType(id);
        accepter(":=");
        emettre1(":=");
        type2=Exp_simple();
    if (!comparer(type1,type2)){
        Erreur_type();
    }
    }
    else if(strcmp(symbole,"if")==0){
        accepter("if");
        type1=Exp();
        etiq=creer_Etiq();
        emettre3("AllerSiFaux",etiq);
        Exp();
        accepter("then");
        I();
        emettre3("Etiq",etiq);
        accepter("else");
        I();
    }
    else if(strcmp(symbole,"while")==0){
        accepter("while");
        Exp();
        accepter("do");
        I();
    }
    else if(strcmp(symbole,"read")==0){
        accepter("read");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else if(strcmp(symbole,"readln")==0){
        accepter("readln");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else if(strcmp(symbole,"write")==0){
        accepter("write");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else if(strcmp(symbole,"writeln")==0){
        accepter("writeln");
        accepter("(");
        accepter("id");
        accepter(")");
    }
    else{
        printf("Erreur");
        exit(0);

    }

}

char Exp() {
    char type1;
    char type2;
    type1=Exp_simple();
    type2=D_prime();
    if(comparer(type2,'v')){
        return(type1);
    else if(comparer(type1,type2)){
        return 'b';
    }
    }else{ Erreur_type()}
}

char C_prime() {
    char type;
    if (strcmp(symbole,"oprel")==0){
        accepter("oprel");
        type=Exp_simple();
        if(strcmp(ch,"<")==0){
            emettre("Comparer-si-inf");
        }else if (strcmp(ch,"<=")==0){
            emettre("Comparer-si-inf-ou-egal");
        }else if(strcmp(ch,">")==0){
            emettre("Comparer-si-sup");
        }else if(strcmp(ch,">=")==0){
            emettre("Comparer-si-sup-ou-egal");
        }else if(strcmp(ch,"=")==0){
            emettre("Comparer-si-egal");
        }else if(strcmp(ch,"<>")==0){
            emettre("Comparer-si-non-egal");
        }
        return type;
    }
    else{return 'v';}
}





char Exp_simple() {
    char type1;
    char type2;
    type1=Terme();
    type2=D_prime();
    if(comparer(type2,'v')){
        return type1;
    }else if(comparer(type1,type2))&&comparer(type1,'i'){
        return 'i';
    }else{Erreur_type();}
}

char D_prime() {
    char type1;
    char type2;
    char chaine[10];

    if (strcmp(symbole,"opadd")==0){
        accepter("opadd");
        type1=Terme();
        emettre1(chaine);
        type2=D_prime();
        if(comparer(type2,'v')){
            return type1;
        }else if(comparer(type1,type2)&&comparer(type1,'i')){
            return 'i';
        }else{Erreur_type();}
    }else{
        return 'v';}
}
char Terme(){
    char type1;
    char type2;
    type1=Facteur();
    type2=F();

    if(comparer(type2,'v')){
        return type1;
    }else if(comparer(type1,type2)&&comparer(type1,'i')){
        return 'i';
    }else{Erreur_type();}
}


char E_prime() {
    char type1;
    char type2;
    char chaine[10];
    if (strcmp(symbole,"opmul")==0){
        accepter("opmul");
        type1=Facteur();
        emettre1(chaine);
        type2=E_prime();
        if(comparer(type2,'v')){
            return type1;
        }else if(comparer(type1,type2)&&type1=='i'){
            return 'i';
        }else{Erreur_type();}
    }else{return 'v';}
}



char Facteur() {
    char type;
    if (strcmp(symbole,"id")==0){
        emettre2("valeurd",table_symbole[variable_Id].symbole);
        accepter("id");
        return chercherType(id);
    }
    else if(strcmp(symbole,"nb")==0){
        accepter("nb");
        return('i');
    }
    else if(strcmp(symbole,"(")==0){
        accepter("(");
        type=Exp_simple();
        accepter(")");
        return type;
    }
    else{
        printf("Erreur");
        exit(0);
    }
}




/*******************************************************/
void ajouterType(int id,char ch){
    if(table_symbole[id].type=='-'){
        table_symbole[id].type=ch;
    }else{
        Erreur_type();
    }
}

void ajouter_plusieurs(char type){
    for (int i=0; i<taille_tab_identificateurs; i++) {
        ajouterType(tab_identificateurs[i],type);
    }
}

char chercherType(int id){
    if(table_symbole[id].type!=NULL){
        return table_symbole[id].type;
    }
}

int comparer(char ch1, char ch2){
    if (ch1==NULL || ch2==NULL){
        return 0;
    }else if(ch1==ch2){
        return 1;
    }
    return 0;
}

void Erreur_type(){
    printf("erreur de type")
    exit(0);
}

void ajouter_tab_identificateurs(char id){
    tab_identificateurs[taille_tab_identificateurs]=id;
    taille_tab_identificateurs++;
}

void emettre1(char *ch){
    fprintf(f_i, "%s \n", ch);
}

void emettre2(char *ch1, char *ch2){
    fprintf(f_i,"%s %s \n", ch1 , ch2 );
}

void emettre3(char *ch1, int c){
    fprintf(f_i,"%s %d \n", ch1 , c );
}

int creer_Etiq(){
    etiquette++;
    return etiquette;
}

/**************************************************************************/


int main() {
    f = fopen("file.txt", "r"); //Lire le code
    f_i = fopen("codeInterm.txt","wt"); //Générer le code intermédiaire
    if (f == NULL) {
        perror("Error opening file");
        return 1;
    }
    P(); //Execute P()
    while(caractere!=EOF){
        printf("%s \n",symbole);
    }

    fclose(f);
    fclose(f_i);
    return 0;
}
