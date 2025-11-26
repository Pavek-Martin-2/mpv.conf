#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// pracuje (edituje) soubor "mpv.conf" pro prehravac "mpv.exe" v ceste filename

using namespace std;

    const string filename = "C:\\Users\\DELL\\AppData\\Roaming\\mpv\\mpv.conf";

    /* lomito je specialni znak (stejne jako na linuxu) 
    takze zde protreba psat takto "\\" misto jednoho lomitka dve
    prikazi v konzoli ve Windows, samozdejme pouze z jednim lomitkem
    
    tady mam v ceste individualni uzivatelsky jmeno "DELL"
	pro zjisteni vasi cesty k souboru "mpv.conf" zadat v terminalu
    prikaz : echo %appdata%\mpv
    nebo : cd %appdata%\mpv
    promenna %appdata% obsahuje retezec "C:\Users\"
	pak je individualni uzivatelske jmeno a pokracuje zese stejne "\AppData\Roaming"
	zde je nekolik slozek, takze je jeste potreba prepnou do adresare "mpv"
	a teprve tady by se mel uz nachazet onen konfiguracni soubor "mpv.conf"
	po kterem a po jeho obsahu se pri kazdem spusteni diva
	command line prehravac "mpv.exe"
    
    radky souboru mpv.conf
    radek 1 jsem vymyslel jako kontrolni, aby se zjistilo co je aktualne nastaveno
    program bude cyklovat mezi pri kazdem jeho dalsim spusteni mezi on/off
    vzdy vypise aktualni obsah souboru a bude cekat v pause na ukonceni
    
    paklize soubor mpv.conf v ceste "%appdata%\mpv\" neexistuje vytvori
    novy a zapise do nej radky pro zapnuti konfigurace conf_1 az conf_5
    paklize by mel soubor "mpv.conf" poskozeni prvni radek, tzn.
    neobsahoval by hodnotu "#on" ani "#off" vytvori soubor pro zapnuti konfigurace
    
    existuji 4 mozne stavy: ( screenshoty viz. adresar "scr\" )
    1) soubor "mpv.conf" neexistuje -> vyptori novy a nastavi na #on
    2) pri spusteni nacetl ze predchozi nastaveni bylo #on -> prepise pro "mpv.exe" na #off
    3) obracene, nacetl predchozi #off -> prepise na @on
    4) vyskytla se chaba a nejde urcit soucasny stav -> nastavi na #on
    
    5) a jaste pata vlastne soubor je ReadOnly a nebo je spatna cesta
       adresar ve kterem ma by soubor "mpv.conf" neexistuje apod.
    */
    
    // deklarace promennych
    string stavovy_radek_conf; // prommena pro kontrolni cteni prvniho radku
    string radek_conf_on = "#on";
    string radek_conf_off = "#off";

    // globalni promenne
    vector<string> pole = {
   "osd-fractions=yes",
   "osd-level=3",
   "osd-font-size=30",
   "osd-playing-msg=${=time-pos} / ${=duration}",
   // sem se moho pridavat jeste dalsi radky, pokud by bylo potreba
   };

    int aa, bb, d_pole;
	
    //funkce zapis config #on
    void zapis_on() {
        // otevreni souboru pro zapis
        ofstream outfile(filename);
        
        if (outfile.is_open()) {
        	
        cout<<filename<<endl<<endl;
        outfile<<radek_conf_on<<endl;
		cout<<radek_conf_on<<endl;
			
        for (aa = 0; aa < d_pole; aa++) {
        outfile<<pole[aa]<<endl;
        cout<<pole[aa]<<endl;
        }
		outfile.close();
		} else {
        cout<<"chyba pri vytvareni souboru"<<endl;
        system("pause");
        exit(1); // opraveno 25.11.2025
        }
    }
    
    /*
    funkce zapis config #off
	radky ktere zacinaji znakem # tak ty bude program "mpv.exe" pri nacetni sve konfigurace
    ignorovat jako zakomentovane, jako kdyby tam nebyly, je to podobne jako treba
    ve skriptu v linuxu, radek tam je aby bylo vydet co tam ma bejt ale nebude se zpracovavat
	a preskoci se, muzou se samozdrejme tako preskocit i vsechny radky souboru
	*/
    void zapis_off() {
        // otevreni souboru pro zapis
        ofstream outfile(filename);
        
        if (outfile.is_open()) {
        	
        cout<<filename<<endl<<endl;
        outfile<<radek_conf_off<<endl;
		cout<<radek_conf_off<<endl;
		
        for (bb = 0; bb < d_pole; bb++) {
        outfile<<"#"<<pole[bb]<<endl;
        cout<<"#"<<pole[bb]<<endl;
        }
		outfile.close();
		} else {
        cout<<"chyba pri vytvareni souboru"<<endl;
        system("pause");
        exit(1); // opraveno 25.11.2025
        }
    }
      
int main() {
	d_pole = pole.size();  // prirazeni az tady
    //cout<<"d_pole = "<<d_pole<<endl;

    // zkusi otevrit soubor pro cteni
    ifstream infile(filename);
    if (!infile.is_open()) { // nelze otevzit soubor
    // soubor neexistuje, vytvorim novy
    zapis_on();
    system("pause");
    exit(0); // tady uz je potreba skoncit a nepustit to dal na dalsi podminky
	// protoze by se neco vykonavalo 2x     
    } else {
    // soubor uz existuje, nacte z nej hodnotu prvniho radku #on nebo #off (pripadne error)
    infile>>stavovy_radek_conf; // kontrolni prvni radek, jak je nastavena konfigurace z predchoziho spusteni
    infile.close();  
    }
    
    // prepinani stavu podle necteneho prvniho radku conf souboru on->off a off->on (cyklovani)
	if (stavovy_radek_conf == "#on"){ // paklize je konfigurace #on prepis soubor na #off
	// zapnuto -> vypnout
    zapis_off();
    }
    
    // obracene, kdyz je #off prepis na #on ale take kdyz je chyba
    // a radek neni ani "#on" ani "#off"
    if ((stavovy_radek_conf == "#off") || (stavovy_radek_conf != "#on" && stavovy_radek_conf != "#off")){
    // vypnuto -> zapnout
    zapis_on();
    }
	
	system("pause");
	return 0;
}


