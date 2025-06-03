/***************************************************************************
 *   Copyright (C) 2008-2017 by Andreas Winter                             *
 *   andreas.f.winter@web.de                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "fundamentals.h"
#include "MainWindow.h"
#include "allWidgets.h"
#include "undo_module.h"

extern MainWindow * mainWin;
extern frmTextProps * TextProps;
extern frmTextProps * EditTextProps;
extern frmLineProps * LineProps;
extern frmLineProps * EditLineProps;
extern frmEllipseProps * BoxProps;
extern frmEllipseProps * EditBoxProps;
extern frmEllipseProps * EllipseProps;
extern frmEllipseProps * EditEllipseProps;
extern frmSetEditor * FormSetEditor;
extern frmEditBlockData * FormEditBlockData;
//extern frmEditColumnProp * EditColumn;
extern QPixmap * GraceIcon;
extern QPixmap * ActiveIcon;
extern QPixmap * HiddenIcon;
extern QIcon ** ColorIcons;
extern QPixmap ** ColorPixmaps;
extern QString ** ColorNames;
/*extern QIcon * LineIcons[MAXLINESTYLES];
extern QPixmap * LinePixmaps[MAXLINESTYLES];*/
extern int nr_of_current_linestyles;
extern int * lenghts_of_linestyle_patterns;
extern char ** current_linestyle_patterns;
extern QIcon ** LineIcons;
extern QPixmap ** LinePixmaps;
extern QVector<qreal> ** PenDashPattern;
extern QBitmap * patterns[MAXPATTERNS];
extern QPixmap * PatternPixmaps[MAXPATTERNS];
extern QPixmap * Qt_justifications[12];
extern QPixmap * Qt_matrixOrder[8];

extern bool useQtFonts;
extern QList<QFont> stdFontList;
extern QFontMetrics * stdFontMetrics;
extern QStringList ListOfChanges;
extern QStringList ListOfOldStates;
extern CMap_entry *cmap_table;
extern int allocated_colors;

extern graph * g;
extern int maxgraph;
extern int new_set_no;
extern int DefaultFont;
extern bool activate_id_replacing;

extern void strToUpper(char * tar,const char * ch);
extern void add_ColorSelector(ColorSelector * colSel);
extern void add_FontSelector(FontSelector * fontSel);
extern void add_LaTeX_Line(stdLineEdit * line);
extern void add_Line_Style_Selector(LineStyleSelector * line);
extern void update_font_selectors(bool appearance);
extern void update_graph_selectors(void);
extern void showSetInSpreadSheet(int gno,int setno);
void close_ss_editor(int gno,int setno);
extern frmLoadEval * FormLoadAndEvaluate;

extern void add_SetSelector(uniList * setSel);
extern void add_GraphSelector(uniList * grSel);
extern void add_SetChoiceItem(uniList * setSel);
extern void HelpCB(char *data);
#ifdef __cplusplus
extern "C" {
#endif
extern double rint_2(double v);
#ifdef __cplusplus
}
#endif
extern char dummy[];//universal dummy for general purposes
extern char dummy2[];
extern bool updateRunning;
QColor * stdTextColor=NULL;

extern char SystemsDecimalPoint;//the default decimal-separator
extern char OldDecimalPoint;
extern char DecimalPointToUse;//what the user wants to use as decimal separator

extern QTextCodec * FileCodec;

extern int maxboxes;
extern int maxlines;
extern int maxstr;
extern int maxellipses;

///todo: check if this is realy needed
QStringList LaTeXCommands;
QStringList equivalentCommands;
QList<char> equivalentFont;

extern int recursive_replacer(QString & text);
extern void GeneralPaste(const QMimeData * mimeData);
extern void complete_LaTeX_to_Grace_Translator(QString & text);
extern QString generateDisplayStringFromGraceString(char * gracestring);
extern void generate_string_Qt_aware(char * str,QString text);

extern int find_QtFont_in_List(const char * name,int whatlist);//you need a qt-font-name for this
extern int find_QtFont_in_List(QString fontname,int whatlist);//finds a qt-font-name
extern int find_default_id_of_Grace_Font(const char * name);//you need a Grace-font-name for this
extern int find_GraceFontID_of_QtFontName(QString fontname);//you need a qt-font-name for this
extern QString getNameOfDefaultQtFont(int index);
extern QString get_QtName_of_Default_Grace_Font(const char * name);//compares a Grace-font-name with the default font names and returns the qt-font-name
extern char * get_Grace_Font_Name_of_Default_Qt_Font(QString fontname);//returns the Grace-font-name of a Qt-Font-name
extern char * getNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
extern char * getFamilyNameOfStdQtFont(int index);//returns the Qt-Name of the Font in the StdList
//this is what you need
//--> remember: whatlist=0-->default-list, whatlist=1-->std-list (i.e. the current list - you probably want to use this)
//--> convert Grace-name into QtFontID
extern int get_QtFontID_from_Grace_Name(const char * name,int whatlist);//get the index of a QtFont in a list by its Grace-name
//--> convert Grace-font-id into QtFontID
extern int get_QtFontID_from_GraceID(int font_id,int whatlist);

extern void copySet(int gno,int setno,plotarr * pa,int what);
extern void reinstallSet(int gno,int setno,plotarr * pa,int what);
extern void deleteSavedSet(plotarr * pa,int what);
extern bool dont_delete_saved_set_memory;

extern void Replace_Dec_Sep_In_Single_String(QString & te);
extern void prepare_highlight_segments_with_graphs(int n,int * gnos);
extern void prepare_highlight_segments_with_sets(int n,int * gnos,int * snos);
extern int get_legend_entry_bb_for_set(int gno, int setno, view * bb);
extern double getOrientationAngle(int type,int gno,int nr);
extern void remove_beginning_whitespaces(QString & text);

void WriteFilterString(QString & filterCommand,int o_n_sets,int * o_gnos,int * o_snos,int n_sets,int * gnos,int * snos,int type,int realization,double * limits,int * orders,char * x_formula,double ripple,int absolute,int debug,int point_extension,int oversampling,int rno,int invr);
void WriteRegressionString(QString & regressionCommand,int n_sets,int * gnos,int * snos,int n_n_sets,int * n_gnos,int * n_snos,int ideg,int rno,int invr,double start,double stop,int points,int rx,char * formula);

int nr_of_set_app_saved=0;
int nr_of_set_app_allocated=0;
plotarr * saved_set_app=NULL;

extern int nr_of_single_f_tokens;
extern class single_formula_token * formula_tokens;

extern char last_formula[];
extern char saved_formula[];
extern double last_fit_falues[];
extern double saved_value;

extern int current_origin_graph,current_origin_set;
extern int current_target_graph,current_target_set;

int slider_status=0;//0=slider inactive; 1=sliding started; 2=slider start accepted; 3=sliding finished

void drawSimpleSymbol(QPainter & paint1,int x,int y,int w,int sym,char sym_c);//paints a symbol on a painter in the size w x w (always a square) on middle-position x,y; color and linestyle have to be set beforehand

using namespace std;

/* lookup table to determine if character is a floating point digit
 * only allowable char's [0-9.eE]
 */
unsigned char fpdigit[256] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

void delete_special_int(int ** target)
{
if (*target==NULL) return;
delete[] *target;
*target=NULL;
}

void resize_special_int(int ** target,int o_size,int n_size)
{
int to_copy=o_size<n_size?o_size:n_size;
if (*target==NULL) to_copy=0;
if (n_size==0)
{
delete[] *target;
return;
}
int * new_target=new int[n_size];
if (*target!=NULL)
{
memcpy(new_target,*target,sizeof(int)*to_copy);
delete[] *target;
}
*target=new_target;
}

void getSetIDFromText(char * text,int & gno,int & sno,int & column)
{
//tries to read a set-id from the text, if incomplete id,
//the gno/sno-values stay untouched (gno/sno should be initialized
//with a suitable default before this function is called or a useless
//value to detect unsuccessfull read-attempts)
int a,b;
char c_text[32];
//qDebug() << "text=" << text;
if (sscanf(text,"G%d.S%d.%s",&a,&b,c_text)==3)
{
    if (c_text[0]=='X' || c_text[0]=='x')
    {
    column=DATA_X;
    }
    else if (c_text[0]=='y' || c_text[0]=='Y')
    {
    column=DATA_Y;
        if (c_text[1]!='\0')
        {
        column+=atoi(c_text+1);
        }
    }
//qDebug() << "0a: a=" << a << " b=" << b << " column=" << column;
gno=a;
sno=b;
}
else if (sscanf(text,"S%d.%s",&b,c_text)==2)
{
    if (c_text[0]=='X' || c_text[0]=='x')
    {
    column=DATA_X;
    }
    else if (c_text[0]=='y' || c_text[0]=='Y')
    {
    column=DATA_Y;
        if (c_text[1]!='\0')
        {
        column+=atoi(c_text+1);
        }
    }
//qDebug() << "0b: b=" << b << " column=" << column;
sno=b;
}
else if (sscanf(text,"G%d.S%d",&a,&b)==2)
{
//qDebug() << "1: a=" << a << " b=" << b;
gno=a;
sno=b;
return;
}
else if (sscanf(text,"G%d",&a)==1)
{
//qDebug() << "2: a=" << a;
gno=a;
return;
}
else if (sscanf(text,"S%d",&b)==1)
{
//qDebug() << "3: b=" << b;
sno=b;
return;
}
}

void sort(int number,int * items)//bubble-sort (I know that I am stupid - but it works)
{
    int dummy;
    for (int i=0;i<number;i++)
    {
        for (int j=i+1;j<number;j++)
        {
            if (items[j]<items[i])
            {
                dummy=items[j];
                items[j]=items[i];
                items[i]=dummy;
            }
        }
    }
}

int indexOfFontInDatabase(QFont & f)
{
    QString s1=f.toString();
    QString s2;
    int index=-1;
    if (s1.compare(QString("Zapf,13,-1,5,50,0,0,0,0,0"))==0)//QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0")
    {
        //s1=QString("Zapf,13,-1,5,50,0,0,0,0,0");
        s1=QString("Zapf Dingbats,10,-1,5,50,0,0,0,0,0");
    }
    for (int i=0;i<stdFontList.length();i++)
    {
        s2=stdFontList.at(i).toString();
        if (s1.compare(s2)==0)
        {
            index=i;
            break;
        }
    }
    return index;
}

QFont getFontFromDatabase(int i)
{
    QFont fallback;
    if (stdFontList.length()==0)
    {
        if (fallback.fromString(QString("Times,10,-1,5,50,0,0,0,0,0"))==false)
        {
            if (fallback.fromString(QString("Times New Roman,10,-1,5,50,0,0,0,0,0"))==false)
            fallback=qApp->font();
        }
        return fallback;
        //return QFont( QString("Times,10,-1,5,50,0,0,0,0,0"));//just for emergencies
    }
    else if (i<0 || i>=stdFontList.length())
        return stdFontList.at(0);
    else
        return stdFontList.at(i);
}

int addFontToDatabase(QFont & f)
{
    int font_index=indexOfFontInDatabase(f);
    if (font_index!=-1)//font already exists --> do not add this font
    {
        //cout << "font " << f.toString().toLatin1().constData() << " already exists=" << font_index << endl;
        return font_index;
    }
    else
    {
        //cout << "new font added=" << f.toString().toLatin1().constData() << endl;
        stdFontList << f;
        return stdFontList.length()-1;//new font has last index
    }
}

void append_to_storage(int * l,int ** storage,int n,int * new_entries)
{
    static int m1,m2;
    int * temp;
    //the memory size of storage automatically increases by DELTASIZE if needed
    //new_entries are appended; len(l) is the current storage size, n the number of new numbers to append
    //if storage==NUll --> storage is empty, new storage space is allocated
    if (n<=0 || new_entries==NULL) return;
    if (*l<=0 && *storage!=NULL)
    {
        delete[] *storage;
        *storage=NULL;
        *l=0;
    }
    m1=(*l)/DELTASIZE;
    m2=(*l+n)/DELTASIZE;
    if (m1*DELTASIZE<*l) m1++;
    if (m2*DELTASIZE<*l+n) m2++;
    if (*storage==NULL)
    {
        *storage=new int[(m2)*DELTASIZE];
        *l=0;
    }
    else if (m2>m1)//more memory needed
    {
        temp=*storage;
        *storage=new int[(m2)*DELTASIZE];
        memcpy(*storage,temp,sizeof(int)*(m1)*DELTASIZE);
        delete[] temp;
    }//else: enough memory
    memcpy(*storage+*l,new_entries,sizeof(int)*n);
    *l+=n;
}

void append_to_storage2(int * l,int ** storage_a,int ** storage_b,int n,int * new_entries_a,int * new_entries_b)
{
int tmp_n=*l;//old storage size
append_to_storage(l,storage_a,n,new_entries_a);
*l=tmp_n;
append_to_storage(l,storage_b,n,new_entries_b);
}

void SetDecimalSeparatorToUserValue(char * str,bool remove_space=true)//we assume a string containing a number like "2.15" and we want to change it to a number with the user selected decimal separator like "2,15"
{
QString chain(str);
    if (remove_space)
    chain.remove(QString(" "));//remove any useless spaces
    if (DecimalPointToUse=='.')
    {
        //chain.replace(QString(","),QString("|"));
        return;//Nothing to do here --> everything is as it should be
    }
    chain.replace(QString(","),QString("|"));//replace the ','s
    chain.replace(QString("."),QString(","));//replace the '.'s
strcpy(str,chain.toLatin1().constData());
}

void ReplaceDecimalSeparator(char * str,bool remove_space=true)//replace current user specified decimal separator with the internally used '.'
{
QString chain(str);
    //cout << "to replace = " << str << endl;
    if (remove_space)
    chain.remove(QString(" "));//remove any useless spaces
    if (DecimalPointToUse!='.')//the input contains ',' instead of '.' - but the system always expects '.'
    {
        //chain.remove(QString("."));//at first: remove '.'
        chain.replace(QString(","),QString("."));
        chain.replace(QString("|"),QString(","));//replace the ',' with '.'
    }
    else//DecimalPoint is '.' - any ',' are not useful
    {
        //chain.remove(QString(","));//just remove all ','
        //chain.replace(QString(","),QString("|"));
        ;//should be ok as it is!
    }
strcpy(str,chain.toLatin1().constData());
    //cout << "result = " << str << endl;
}

void RedisplayString(char * str)//replaces the old decimal separator with the new one
{
    if (OldDecimalPoint==DecimalPointToUse) return;
    QString chain(str);
    Replace_Dec_Sep_In_Single_String(chain);
    strcpy(str,chain.toLatin1().constData());
}

void PrepareFormula(char * str)//replaces the decimal separator in a formula with the '.' for internal calculations
{
    QString chain(str);
    ///chain.toUpper();///toUpper deactivated at the moment
    if (DecimalPointToUse==',')//if '.' is set --> nothing to do
    {//if ',' is used --> just replace ','-->'.'
        chain.replace(QChar(','),QChar('.'));
        chain.replace(QChar('|'),QChar(','));
    }
    strcpy(str,chain.toLatin1().constData());
}

void RedisplayFormula(char * str)//replaces the decimal separator except in the set-ids: e.g. G0.S4.Y
{
    QRegExp rex1("G\\d,S\\d,");
    QRegExp rex2("S\\d,");
    int pos,pos2;
    QString chain(str);
    ///chain.toUpper();///toUpper deactivated at the moment
    if (OldDecimalPoint==',' && DecimalPointToUse=='.')//the simple case
    {
        chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
        chain.replace(QChar('|'),QChar(','));
    }
    else if (OldDecimalPoint=='.' && DecimalPointToUse==',')//the complicated case
    {
        chain.replace(QChar(','),QChar('|'));
        chain.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));//this replaces the decimal separators as well as the set-id-separators: G0.S4.Y-->G0,S4,Y
        while ((pos=rex1.indexIn(chain))>=0)
        {
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
        }
        while ((pos=rex2.indexIn(chain))>=0)
        {
            pos2=chain.indexOf(",",pos);
            chain[pos2]='.';
        }
    }
    strcpy(str,chain.toLatin1().constData());
}

int GetIntValueFromString(char * str)
{
    ReplaceDecimalSeparator(str);
    return atoi(str);
}

double GetDoubleValueFromString(char * str)
{
    ReplaceDecimalSeparator(str);
    return atof(str);
}

struct komplex add_komplex(struct komplex a,struct komplex b)
{
    struct komplex c={a.real+b.real,a.imag+b.imag};
    return c;
}

struct komplex sub_komplex(struct komplex a,struct komplex b)
{
    struct komplex c={a.real-b.real,a.imag-b.imag};
    return c;
}

struct komplex mult_komplex(struct komplex a,struct komplex b)
{
    //(ar+i*ai)*(br+i*bi)=(ar*br-ai*bi)+i*(ai*br+bi*ar)
    struct komplex c={a.real*b.real-a.imag*b.imag,a.imag*b.real+b.imag*a.real};
    return c;
}

struct komplex div_komplex(struct komplex a,struct komplex b)
{
    //(ar+i*ai)/(br+i*bi)=(ar+i*ai)/(br+i*bi)*(br-i*bi)/(br-i*bi)=((ar*br+ai*bi)+i*(ai*br-bi*ar))/(br*br+bi*bi)
    double betr=b.real*b.real+b.imag*b.imag;
    struct komplex c={(a.real*b.real+a.imag*b.imag)/betr,(a.imag*b.real-b.imag*a.real)/betr};
    return c;
}

struct komplex pow_komplex(struct komplex a,double n)
{
//static double pi=2.0*asin(1.0);
    double absval=pow(hypot(a.real,a.imag),n);
    double phase=atan2(a.imag,a.real)*n;
    /*if (a.real==0.0)
{
if (a.imag>=0.0) phase=0.5*pi;
else phase=1.5*pi;
}*/
    struct komplex c={absval*cos(phase),absval*sin(phase)};
    return c;
}

void SetLineEditToMemory(char * t1,char * t2,char * &c1,char * &c2,bool &displayStd,QLineEdit * lenText)
{
    static QPalette pal;
    c1=t1;//t1 and c1 are the grace-version of a string that is used internally!
    c2=t2;//t2 and c2 are the originals -- internally the original is always stored in UTF8 (this only changes before saving and after loading in which case the selected codec for external usage is used)
    displayStd=true;
    pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    pal.setColor(QPalette::Text,*stdTextColor);
    if (t2!=NULL)//we display t2(the original) as a standard
    {
        lenText->setText(QString::fromUtf8(t2));//t2 = original = UTF8-encoding
        //if (useQtFonts==false)
        //lenText->setText(FileCodec->toUnicode(t2));//t2 = original
        //else
        //lenText->setText(FileCodec->toUnicode(t2));//t2 = original
    }
    else
    {
        lenText->setText("");
    }
    lenText->setPalette(pal);
}

void ClickedOnLabel(char * c1,char * c2,bool &displayStd,QLineEdit * lenText)
{
    static QPalette pal;
    if (c1==NULL || c2==NULL) return;
    displayStd=!displayStd;
    pal=lenText->palette();
    if (displayStd)//Std is the original user input (probably in LaTeX-format - but always in UTF8-encoding)
    {
        lenText->setText(QString::fromUtf8(c2));
        //if (useQtFonts==false)
        //lenText->setText(QString(c2));
        //else
        //lenText->setText(FileCodec->toUnicode(c2));
        pal.setColor(QPalette::Text,*stdTextColor);
    }
    else
    {
        lenText->setText(generateDisplayStringFromGraceString(c1));
        //if (useQtFonts==false)
        //lenText->setText(QString(c1));//converted input (Grace-format)
        //else
        //lenText->setText(FileCodec->toUnicode(c1));
        pal.setColor(QPalette::Text,Qt::red);
    }
    lenText->setPalette(pal);
}

void SetMemoryToText(char * t1,char * t2,QString text)
{
    strcpy(t2,text.toUtf8().constData());//t2 is always the original - internally always in UTF8
    if (activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(text);
    generate_string_Qt_aware(t1,text);//t1 is the Grace-version of the text
}

void DynSetMemoryToLineEdit(char * &t1,char * &t2,char *&c1,char * &c2,bool & displayStd,bool acceptLaTex,QLineEdit * lenText)
{
    static QPalette pal;
    static QString text;
    text=lenText->text();
    t2 = copy_string(t2,lenText->text().toUtf8().constData());//t2 is always the original - always internally in UTF8
    pal=lenText->palette();
    displayStd=true;
    char * te=new char[MAX_STRING_LENGTH];//longer is not allowed! Do not write essays in Grace ;-) !
    if (acceptLaTex==true && activateLaTeXsupport==TRUE)
    {
        complete_LaTeX_to_Grace_Translator(text);//make the LaTeX-to-Grace-Translation
    }
    generate_string_Qt_aware(te,text);
    t1 = copy_string(t1,te);//t1 is the Grace-version of the text
    c1=t1;
    c2=t2;
    pal.setColor(QPalette::Text,*stdTextColor);
    delete[] te;
}

SetCombo::SetCombo(QWidget * parent):QComboBox(parent)
{
    show_none=true;
    gno=-1;
    nr_of_entries=0;
    snos=new int[2];
}

void SetCombo::update_entries(int gr,bool preserve_selection)
{
    if (is_valid_gno(gr)==FALSE) return;
    int old=value();
    gno=gr;
    delete[] snos;
    snos=new int[3+g[gno].maxplot];
    clear();
    if (show_none==true)
    {
        snos[0]=-1;
        addItem(tr("None"));
        nr_of_entries=1;
    }
    else
        nr_of_entries=0;
    for (int i=0;i<g[gno].maxplot;i++)
    {
        if (is_set_active(gno,i)==true)
        {
            sprintf(dummy,"S%d",i);
            addItem(QString(dummy));
            snos[nr_of_entries++]=i;
        }
    }
    if (old!=-10 && preserve_selection==true)//reset old selection
    {
        for (int i=0;i<nr_of_entries;i++)
        {
            if (snos[i]==old)
            {
                setCurrentIndex(i);
                break;
            }
        }
    }
}

int SetCombo::value(void)
{
    if (gno==-1) return -10;
    else
        return snos[currentIndex()];
}

void SetCombo::setValue(int set)
{
    for (int i=0;i<nr_of_entries;i++)
    {
        if (snos[i]==set)
        {
            setCurrentIndex(i);
            break;
        }
    }
}

SetSelectorCombo::SetSelectorCombo(QString text,QWidget * parent):QWidget(parent)
{
    cmb=new SetCombo(this);
    lblCombo=new QLabel(text,this);
    layout=new QHBoxLayout;
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(lblCombo);
    layout->addWidget(cmb);
    setLayout(layout);
}

void SetSelectorCombo::update_entries(int gno,bool preserve_selection)
{
    cmb->update_entries(gno,preserve_selection);
}

int SetSelectorCombo::value(void)
{
    return cmb->value();
}

void SetSelectorCombo::setValue(int set)
{
    cmb->setValue(set);
}

Panel::Panel(QPixmap * pix,QWidget * parent):QLabel(parent)
{
    p=pix;
    inpaint=false;
    setMouseTracking(TRUE);
}

void Panel::paintEvent(QPaintEvent *event)
{
    if (inpaint==false)
        inpaint=true;
    else
        return;
    QPainter paint(this);
    paint.drawPixmap(0,0,*p);
    paint.end();
    QLabel::paintEvent(event);
    inpaint=false;
}

void Panel::mouseMoveEvent( QMouseEvent * event )
{
    (void)event;
    emit(mouseMoving());
}

void Panel::mouseReleaseEvent(QMouseEvent *event)
{
    (void)event;
    emit(mouseClicked());
}

panelWindow::panelWindow(int rows,int cols,int last_col,QPixmap ** pix,QWidget * parent,bool hasAlphaSlider):QWidget(parent)//QDialog(parent)
{
    if (hasAlphaSlider==true)
    {
    alphaSlider=new horizontalAlphaSlider(this);
    }
    else
    {
    alphaSlider=NULL;
    }
    frm=new QFrame(this);
    layout0=new QHBoxLayout(this);
    layout0->setMargin(0);
    layout0->setSpacing(0);
    frm->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
    signalMapper1=new QSignalMapper;
    signalMapper2=new QSignalMapper;
    pixmaps=pix;
    row_count=rows;
    last_col_count=last_col;
    col_count=cols;
    number_of_elements=row_count*(col_count-1)+last_col_count;
    elementWidth=pix[0]->width();
    elementHeight=pix[0]->height();
    marked=highlighted=-1;//-1 means nothing ist marked or highlighted
    layout=new QGridLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    int r=0,c=0;
    lblPanel=new class Panel*[number_of_elements];
    //cout << "number_of_elements: " << number_of_elements << endl;
    for (int i=0;i<number_of_elements;i++)
    {
        lblPanel[i]=new class Panel(pixmaps[i],this);
        lblPanel[i]->setMinimumSize(elementWidth,elementHeight);

        connect(lblPanel[i], SIGNAL(mouseMoving()), signalMapper1, SLOT(map()));
        connect(lblPanel[i], SIGNAL(mouseClicked()), signalMapper2, SLOT(map()));
        signalMapper1->setMapping(lblPanel[i],i);
        signalMapper2->setMapping(lblPanel[i],i);

        layout->addWidget(lblPanel[i],r,c);
        r++;
        if (r>=row_count)
        {
            r=0;
            c++;
        }
    }
    if (alphaSlider!=NULL)
    {
    layout->addWidget(alphaSlider,row_count,0,1,col_count);
    }
    connect(signalMapper1, SIGNAL(mapped(int)),this, SLOT(mouseMovedIn(int)));
    connect(signalMapper2, SIGNAL(mapped(int)),this, SLOT(mouseClickedIn(int)));
    clickTime=new QTime();
    clickTime->start();
    frm->setLayout(layout);
    setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
    //setWindowFlags(Qt::Popup);
    setMouseTracking(true);
    layout0->addWidget(frm);
    setLayout(layout0);
    /// setModal(false);/// Changed to false for v.0.2.6!! Does not need to be modal anymore, because focus-out-event works properly now.
}

panelWindow::~panelWindow()
{
    if (alphaSlider!=NULL) delete alphaSlider;
    for (int i=0;i<number_of_elements;i++)
        delete lblPanel[i];
    delete[] lblPanel;
    delete clickTime;
    delete signalMapper1;
    delete signalMapper2;
    delete layout0;
    delete layout;
    delete frm;
}

void panelWindow::setAlpha_extern(int a)
{
    alpha=a;
    alphaSlider->slider->setValue(a);
}

void panelWindow::setMarked(int i)
{
    if (marked==i) return;
    if (marked!=-1)
    {
        lblPanel[marked]->setFrameShape(QFrame::NoFrame);
        lblPanel[marked]->repaint();
    }
    marked=i;
    lblPanel[marked]->setFrameShape(QFrame::Panel);
    lblPanel[marked]->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
    lblPanel[marked]->repaint();
}

void panelWindow::mouseMovedIn(int i)
{
/// qDebug() << "mouseMovedIn: i=" << i << " highlighted=" << highlighted << " marked=" << marked << " slider_has_mouse=" << (alphaSlider==QWidget::mouseGrabber()?"YES":"NO");
    if (alphaSlider!=NULL && alphaSlider->hasFocus())
    {
    alphaSlider->releaseMouse();
    this->grabMouse();
        if (i!=-1)
        lblPanel[i]->setFocus();
    }
    if (i==highlighted) return;
    //a new one is to be highlighted
    if (highlighted!=-1)
    {
        if (highlighted==marked)
        {
            lblPanel[highlighted]->setFrameShape(QFrame::Panel);
            lblPanel[highlighted]->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
            lblPanel[highlighted]->repaint();
        }
        else
        {
            lblPanel[highlighted]->setFrameShape(QFrame::NoFrame);
            lblPanel[highlighted]->repaint();
        }
    }
    if (i!=-1)
    {
        lblPanel[i]->setFrameShape(QFrame::Panel);
        lblPanel[i]->setFrameStyle(QFrame::Sunken | QFrame::WinPanel);
        lblPanel[i]->repaint();
    }
    highlighted=i;
    if (alphaSlider!=NULL && alphaSlider->hasFocus())
    {
    this->setFocus();
    this->grabMouse();
        if (i!=-1)
        lblPanel[i]->setFocus();
    }
/// qDebug() << "Moved In: panel=" << i;
}

void panelWindow::leaveEvent( QEvent * event )
{
    (void)event;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
    /// qDebug() << "PanelWindow: Entered alphaSlider";
alphaSlider->setFocus();
return;
}
    if (highlighted!=-1)
    {
        if (highlighted==marked)
        {
            lblPanel[highlighted]->setFrameShape(QFrame::Panel);
            lblPanel[highlighted]->setFrameStyle(QFrame::Raised | QFrame::WinPanel);
            lblPanel[highlighted]->repaint();
        }
        else
        {
            lblPanel[highlighted]->setFrameShape(QFrame::NoFrame);
            lblPanel[highlighted]->repaint();
        }
    }
    highlighted=-1;
/// qDebug() << "Leave PanelWindow";
}

void panelWindow::mouseClickedIn(int i)
{
    hide();
/// qDebug() << "Panel clicked in = " << i << " alpha=" << alpha << " alphaSlider.value=" << alphaSlider->slider->value();
    emit(newSelection(i));
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "E";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alpha));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "E alpha=" << alpha;
}

void panelWindow::mousePressEvent(QMouseEvent *event)
{
cur_element=element(QCursor::pos().x(),QCursor::pos().y());
/// qDebug() << "PressedOn " << cur_element;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
this->releaseMouse();
alphaSlider->setFocus();
event->ignore();
/// qDebug() << "MousePress: PanelWindow: Inside alphaSlider";
/// qDebug() << "PanelWindow.Focus=" << this->hasFocus();
/// qDebug() << "alphaSlider.Focus=" << alphaSlider->hasFocus();
}
else
{
this->grabMouse();
event->accept();
pressed=true;
/// qDebug() << "MousePress: PanelWindow: x=" << event->x() << " y=" << event->y();
}
}

void panelWindow::mouseReleaseEvent(QMouseEvent *event)
{
static int curTime;
cur_element=element(QCursor::pos().x(),QCursor::pos().y());
/// qDebug() << "ReleasedOn " << cur_element;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
this->releaseMouse();
alphaSlider->setFocus();
event->ignore();
/// qDebug() << "MouseRelease: PanelWindow: Inside alphaSlider";
/// qDebug() << "PanelWindow.Focus=" << this->hasFocus();
/// qDebug() << "alphaSlider.Focus=" << alphaSlider->hasFocus();
return;
}
else
this->grabMouse();
/// qDebug() << "MouseRelease: Pos=" << event->pos().y() << " height=" << row_count*elementHeight;
    if (pressed==false) return;
    curTime=clickTime->restart();
    if (curTime<300)
    {
        pressed=false;
        return;
    }
    //int cur_element=element(movex,movey);
    /*int x=event->x();
    int y=event->y();
    x=movex;
    y=movey;
    int r=y/elementHeight;
    int c=x/elementWidth;
    int element=r+c*row_count;*/
/// qDebug() << "Release: PanelWindow: x=" << x << " y=" << y << " xmax=" << col_count*elementWidth << " ymax=" << row_count*elementHeight;
    //if (x<0 || y<0 || x>col_count*elementWidth || y>row_count*elementHeight)
    if (cur_element<0)
    {
        //element=-1;
        if (marked>=0)
            lblPanel[marked]->mouseReleaseEvent(event);
/// qDebug() << "a: marked=" << marked;
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "D";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "D alpha=" << alpha;
    }
    releaseMouse();
    if (cur_element>=0 && cur_element<number_of_elements)
    {
        lblPanel[cur_element]->mouseReleaseEvent(event);
/// qDebug() << "b: element=" << element;
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "A";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "A alpha=" << alpha;
    hide();
    }
    //hide();
}

void panelWindow::mouseMoveEvent(QMouseEvent * event)
{
cur_element=element(QCursor::pos().x(),QCursor::pos().y());
/// qDebug() << "MovedOn " << cur_element;
if (alphaSlider!=NULL && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos())))
{
this->releaseMouse();
alphaSlider->setFocus();
event->ignore();
/// qDebug() << "MouseMove: PanelWindow: Inside alphaSlider";
/// qDebug() << "PanelWindow.Focus=" << this->hasFocus();
/// qDebug() << "alphaSlider.Focus=" << alphaSlider->hasFocus();
return;
}
else
{
this->grabMouse();
}
    /*int x=event->x();
    int y=event->y();
    int r=y/elementHeight;
    int c=x/elementWidth;*/
    //int cur_element=r+c*row_count;
    movex=event->x();
    movey=event->y();
    //cur_element=element(movex,movey);
    //if (x<0 || y<0) element=-1;
    //if (x>col_count*elementWidth || y>row_count*elementHeight) element=-1;
    if (cur_element<0 || cur_element>=number_of_elements)
    {
        leaveEvent(event);
    }
    else
    {
        if (cur_element!=highlighted)
        {
            lblPanel[cur_element]->mouseMoveEvent(event);
        }
    }
    this->mouseMovedIn(cur_element);
event->accept();
}

void panelWindow::keyPressEvent(QKeyEvent * event)
{
    if (event->key()==Qt::Key_Escape)
    {
    event->accept();
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "B";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "B alpha=" << alpha;
    hide();
    }
}

void panelWindow::focusOutEvent(QFocusEvent * event)
{
    if (alphaSlider!=NULL && alphaSlider->hasFocus() && alphaSlider->rect().contains(alphaSlider->mapFromGlobal(QCursor::pos()))) return;
/// qDebug() << "PanelWindow: FocusOut";
    event->accept();
        if (alphaSlider!=NULL && alpha!=alphaSlider->slider->value())
        {
        //qDebug() << "C";
        alpha=alphaSlider->slider->value();
        emit(newAlpha(alphaSlider->slider->value()));
        }
        //else if (alpha==alphaSlider->slider->value())
        //qDebug() << "C alpha=" << alpha;
    hide();
}

int panelWindow::element(int x,int y)
{
QPoint pos1(x,y);
QPoint pos2;
int nr=-1;
for (int i=0;i<number_of_elements;i++)
{
    pos2=lblPanel[i]->mapFromGlobal(pos1);
    if (lblPanel[i]->rect().contains(pos2))
    {
    nr=i;
    break;
    }
}
if (nr==-1 && alphaSlider!=NULL)
{
pos2=alphaSlider->mapFromGlobal(pos1);
    if (alphaSlider->rect().contains(pos2))
    {
    nr=-2;
    }
}
return nr;
}

mySlider::mySlider(Qt::Orientation orientation, QWidget *parent):QSlider(orientation,parent)
{
}

void mySlider::focusOutEvent(QFocusEvent * event)
{
//qDebug() << "Focus Out Event";
event->accept();
emit(focusOut());
}

verticalSliderPopup::verticalSliderPopup(int width,QWidget * parent):QWidget(parent)
{
//slider=new QSlider(Qt::Vertical,this);
slider=new mySlider(Qt::Vertical,this);
connect(slider,SIGNAL(focusOut()),this,SLOT(focusOut()));
//setWindowFlags(Qt::Dialog|Qt::FramelessWindowHint);
setWindowFlags(Qt::Popup);
setMouseTracking(false);
width_hint=width;
visible_timer=new QTime();
visible_timer->start();
}

void verticalSliderPopup::setRange(int min_v,int max_v)
{
slider->setRange(min_v,max_v);
}

void verticalSliderPopup::showEvent(QShowEvent * event)
{
this->blockSignals(true);
slider->blockSignals(true);
event->accept();
    this->resize(width_hint,5*width_hint);
    slider->setGeometry(STD_MARGIN,STD_MARGIN,width_hint-2*STD_MARGIN,width_hint*5-2*STD_MARGIN);
    this->move(parentWidget()->mapToGlobal(QPoint(0,parentWidget()->height())));
visible_timer->restart();
//qDebug() << "vert. Slider: Show Event";
slider->blockSignals(false);
this->blockSignals(false);
}

void verticalSliderPopup::focusOutEvent(QFocusEvent * event)
{
    if (slider->isVisible()==false || this->rect().contains(this->mapFromGlobal(QCursor::pos()))) return;
event->accept();
hide();
visible_timer->restart();
//qDebug() << "vertical Slider focus Out";
}

void verticalSliderPopup::focusOut(void)
{
    if (slider->isVisible()==false || this->rect().contains(this->mapFromGlobal(QCursor::pos()))) return;
hide();
visible_timer->restart();
//qDebug() << "focus_out";
}

alphaFrame::alphaFrame(QWidget * parent, bool hasSlider):QFrame(parent)
{
setFrameStyle(QFrame::Raised | QFrame::WinPanel);
if (hasSlider)
{
slider=new verticalSliderPopup(this->width(),this);
slider->setRange(0,255);
slider->hide();
connect(slider->slider,SIGNAL(sliderMoved(int)),this,SLOT(setAlpha(int)));
connect(slider->slider,SIGNAL(valueChanged(int)),this,SLOT(setAlpha(int)));
}
else
{
slider=NULL;
}
alpha=255;
setMaximumSize(20,20);
setToolTip(tr("Set opacity (alpha-channel)"));
}

void alphaFrame::mousePressEvent( QMouseEvent * event )

{
    if (slider==NULL) return;
slider->slider->blockSignals(true);
slider->blockSignals(true);
event->accept();
int restart_time=slider->visible_timer->restart();
//qDebug() << "alphaFrame: mousePress: restart_time=" << restart_time << " slider.visible=" << slider->isVisible();
    if (slider->isVisible() || restart_time<100)
    {
    ;//slider->hide();
    }
    else
    {
    slider->width_hint=this->width();
    slider->slider->setValue(alpha);
    slider->show();
    slider->raise();
    slider->activateWindow();
    slider->slider->setFocus();
    //slider->setFocus();
    }
slider->blockSignals(false);
slider->slider->blockSignals(false);
}

void alphaFrame::mouseReleaseEvent( QMouseEvent * event )
{
event->accept();
    return;
//if (slider==NULL) return;
}

void alphaFrame::paintEvent(QPaintEvent * event)
{
QPainter * painter=new QPainter(this);
QColor col(Qt::white);
    painter->setPen(col);
    painter->setBrush(col);
    painter->drawRect(0,0,width(),height());
col.setRgb(0,0,0,alpha);
    painter->setPen(col);
    painter->setBrush(col);
    painter->drawRect(2,2,width()-5,height()-5);
    painter->end();
    event->accept();
}

void alphaFrame::resizeEvent( QResizeEvent * event )
{
repaint();
    if (slider!=NULL)
    if (slider->isVisible())
    {
    slider->show();
    }
event->accept();
}

QSize alphaFrame::sizeHint() const
{
return QSize(10,10);
}

void alphaFrame::setAlpha_extern(int a)
{
alpha=a;
repaint();
}

void alphaFrame::setAlpha(int a)
{
setAlpha_extern(a);
emit(valueChanged(a));
}

horizontalAlphaSlider::horizontalAlphaSlider(QWidget * parent):QWidget(parent)
{
layout=new QHBoxLayout(this);
layout->setSpacing(0);
layout->setMargin(STD_MARGIN);

lblAlpha=new alphaFrame(this,false);
lblAlpha->setAlpha_extern(255);
lblAlpha->setMinimumSize(18,18);
lblAlpha->setToolTip(QString());

slider=new QSlider(Qt::Horizontal,this);
slider->setRange(0,255);
slider->setValue(255);
lblSlider=new QLabel(tr("Opacity:"),this);

layout->addWidget(lblSlider);
layout->addWidget(slider);
layout->addWidget(lblAlpha);

setLayout(layout);
connect(slider,SIGNAL(valueChanged(int)),SLOT(setAlpha(int)));
setMouseTracking(true);
}

void horizontalAlphaSlider::setAlpha(int a)
{
lblAlpha->setAlpha_extern(a);
//qDebug() << "setAlpha=" << a;
emit(alphaChanged(a));
}

void horizontalAlphaSlider::setAlpha_extern(int a)
{
slider->setValue(a);
}

void horizontalAlphaSlider::mousePressEvent( QMouseEvent * event )
{
//qDebug() << "horizontal: press at " << this->mapFromGlobal(QCursor::pos());
    if (!this->rect().contains(this->mapFromGlobal(QCursor::pos())))
    {
        releaseMouse();
        event->ignore();
        parentWidget()->grabMouse();
    }
}

void horizontalAlphaSlider::mouseReleaseEvent( QMouseEvent * event )
{
//qDebug() << "horizontal: release at " << this->mapFromGlobal(QCursor::pos());
    if (!this->rect().contains(this->mapFromGlobal(QCursor::pos())))
    {
        releaseMouse();
        event->ignore();
        parentWidget()->grabMouse();
    }
}

void horizontalAlphaSlider::leaveEvent( QEvent * event )
{
//qDebug() << "horizontal: leaveEvent";
this->releaseMouse();
parentWidget()->grabMouse();
parentWidget()->setFocus();
parentWidget()->setMouseTracking(true);
event->accept();
}

newCombo::newCombo(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly,QWidget * parent,bool hasAlphaSlider):QComboBox(parent)
{
    text_only=showTextOnly;
    simpleColorIcons=NULL;
    if (text_only==false)
    {
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));
    }
    panels=new panelWindow(rows,cols,last_col,pix,this,hasAlphaSlider);//actually initialize the panels
    panels->hide();
    connect(panels,SIGNAL(newSelection(int)),SLOT(wrapperSetCurrentIndex(int)));
    title_strings=new QString[panels->number_of_elements];
    for (int i=0;i<panels->number_of_elements;i++)
        title_strings[i]=*(titles[i]);
    setMinimumWidth(110);
    createSimpleColorIcons();
    setCurrentIndex(0);
}

QIcon createSingleSmallColorIcon(int col)
{
QPixmap map1(12,12);
QPainter paint1;
QColor tmp_color;
    tmp_color.setRgb(cmap_table[col].rgb.red,cmap_table[col].rgb.green,cmap_table[col].rgb.blue);
    paint1.begin(&map1);
    paint1.setBrush(tmp_color);
    paint1.setPen(Qt::NoPen);
    paint1.drawRect(0,0,13,13);
    paint1.end();
return QIcon(map1);
}

QIcon createSingleSmallLineStyleIcon(int col)
{
QPixmap map1(22,12);
QPainter paint1;
QPen pen;
paint1.begin(&map1);
paint1.setBrush(Qt::white);
paint1.setPen(Qt::white);
paint1.drawRect(0,0,29,13);
    if (col>0)
    {
    //qDebug() << "col=" << col << " count=" << PenDashPattern[0]->count();
    pen.setDashPattern(*PenDashPattern[col]);
    pen.setColor(Qt::black);
    pen.setWidth(1);
    paint1.setPen(pen);
    paint1.drawLine(0,6,29,6);
    }
paint1.end();
return QIcon(map1);
}

void newCombo::createSimpleColorIcons(void)
{
    /* small color icons */
    if (simpleColorIcons!=NULL) delete[] simpleColorIcons;
    simpleColorIcons=new QIcon[panels->number_of_elements];
    /*QPixmap map1(12,12);
    QPainter paint1;
    QColor tmp_color;*/
int limit=(unsigned int)panels->number_of_elements<number_of_colors()?panels->number_of_elements:(int)number_of_colors();
    for (int i=0;i<limit;i++)
    {
    //cout << "Color " << i << "/" << number_of_colors() << "=" << cmap_table[i].rgb.red << "|" << cmap_table[i].rgb.green << "|" << cmap_table[i].rgb.blue << endl;
    simpleColorIcons[i]=createSingleSmallColorIcon(i);
    /*
    tmp_color.setRgb(cmap_table[i].rgb.red,cmap_table[i].rgb.green,cmap_table[i].rgb.blue);
    paint1.begin(&map1);
    paint1.setBrush(tmp_color);
    paint1.setPen(Qt::NoPen);
    paint1.drawRect(0,0,13,13);
    paint1.end();
    simpleColorIcons[i]=QIcon(map1);
    */
    }
}

void newCombo::reinitializePanels(int rows,int cols,int last_col,QPixmap ** pix,QString ** titles,bool showTextOnly,bool hasAlphaSlider)
{
    text_only=showTextOnly;
    if (text_only==false)
        setIconSize(QSize(pix[0]->width(),pix[0]->height()));
    //first: delete old panels
    delete panels;//call destructor
    //second: create new panels
    panels=new panelWindow(rows,cols,last_col,pix,this,hasAlphaSlider);//reinitialize the panels
    panels->hide();
    connect(panels,SIGNAL(newSelection(int)),SLOT(wrapperSetCurrentIndex(int)));
    //third: create new titles
    delete[] title_strings;
    title_strings=new QString[panels->number_of_elements];
    for (int i=0;i<panels->number_of_elements;i++)
        title_strings[i]=*(titles[i]);
    createSimpleColorIcons();
    setCurrentIndex(0);
}

void newCombo::wrapperSetCurrentIndex(int index)
{
    setCurrentIndex(index);
    emit(current_Index_Changed(index));
}

void newCombo::setCurrentIndex(int index)
{
    if (index<0 || index>=panels->number_of_elements)
    {
        cout << "Selection Index out of range!" << endl;
        return;
    }
    panels->setMarked(index);
    selection=index;
    clear();
    if (text_only==true)
    {
        if (icon_preferences==0 || simpleColorIcons==NULL)//text only
        addItem(title_strings[index]);
        else if (icon_preferences==1)//text + icons
        addItem(simpleColorIcons[index],title_strings[index]);
        else//icons only
        addItem(simpleColorIcons[index],QString(""));
    }
    else
    {
        addItem(QIcon(*panels->pixmaps[index]),"");
    }
    QMouseEvent * a=new QMouseEvent(QEvent::MouseButtonRelease,QPoint(0,0),Qt::LeftButton,Qt::LeftButton,Qt::NoModifier);
    QComboBox::mouseReleaseEvent(a);
}

int newCombo::currentIndex(void)
{
    return selection;
}

void newCombo::showPopup()
{
    int row_number=0;
    int col_number=0;
    for (int i=0;i<selection;i++)
    {
        row_number++;
        if (row_number>=panels->row_count)
        {
            row_number=0;
            col_number++;
        }
    }
    panels->setMarked(selection);
    panels->move(mapToGlobal(QPoint(-col_number*panels->elementWidth,-row_number*panels->elementHeight)));
    panels->show();
    //panels->pressed=false;
    panels->clickTime->restart();
    panels->pressed=true;
    panels->repaint();
    qApp->processEvents();
    panels->grabMouse();/// reactivated this for v0.2.6
    panels->setFocus();
    //hidePopup();
    qApp->processEvents();
}

void newCombo::wheelEvent(QWheelEvent * e)
{
#if QT_VERSION >= 0x050000
double degrees=e->angleDelta().y()/8.0/15.0;
#else
double degrees=double(e->delta())/8.0/15.0;
#endif
int val,max;
    if (degrees!=0.0)
    {
        val=currentIndex();
        max=panels->number_of_elements;
        if (degrees>0)//scroll up
        {
        val--;
        }
        else if (degrees<0)//scroll down
        {
        val++;
        }
        if (val>=max) val=max-1;
        if (val<0) val=0;
        if (val!=currentIndex())
        {
        blockSignals(true);
        setCurrentIndex(val);
        blockSignals(false);
        emit(current_Index_Changed(val));
        }
    }
}

fitLine::fitLine(QWidget * parent,int nr):QWidget(parent)
{
    char dummy[128];
    sprintf(dummy,"A%d:",nr);
    lblA=new QLabel(QString(dummy),this);
    sprintf(dummy,"< A%d <",nr);
    lblABounds=new QLabel(QString(dummy),this);
    chkBonds=new QCheckBox(tr("Bounds:"),this);
    connect(chkBonds,SIGNAL(stateChanged(int)),this,SLOT(constr_check(int)));
    ledValue=new QLineEdit(QString("1"),this);
    ledLowerBound=new QLineEdit(QString("1"),this);
    ledHighterBound=new QLineEdit(QString("1"),this);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblA);
    layout->addWidget(ledValue);
    layout->addWidget(chkBonds);
    layout->addWidget(ledLowerBound);
    layout->addWidget(lblABounds);
    layout->addWidget(ledHighterBound);
    setLayout(layout);
}

void fitLine::Redisplay(void)
{
    if (OldDecimalPoint==DecimalPointToUse) return;
    QString te;
    te=ledValue->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledValue->setText(te);
    te=ledLowerBound->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledLowerBound->setText(te);
    te=ledHighterBound->text();
    Replace_Dec_Sep_In_Single_String(te);
    //te.replace(QChar(OldDecimalPoint),QChar(DecimalPointToUse));
    ledHighterBound->setText(te);
}

void fitLine::constr_check(int t)
{
    if (t==0)
    {
        ledLowerBound->setEnabled(false);
        ledHighterBound->setEnabled(false);
        lblABounds->setEnabled(false);
    }
    else
    {
        ledLowerBound->setEnabled(true);
        ledHighterBound->setEnabled(true);
        lblABounds->setEnabled(true);
    }
}

void fitLine::getValues(double & value,bool & active,double & lowerBound,double & upperBound)
{
    value=atof(ledValue->text().toLatin1());
    active=chkBonds->isChecked()==TRUE?true:false;
    lowerBound=atof(ledLowerBound->text().toLatin1());
    upperBound=atof(ledHighterBound->text().toLatin1());
}

axisLine::axisLine(QWidget * parent,int nr):QWidget(parent)
{
    original=true;
    c1=c2=NULL;
    sprintf(dummy,"%d",nr);
    lblNr=new QLabel(QString(dummy),this);
    ledLocation=new QLineEdit(QString(""),this);
    ledLabel=new QLineEdit(QString(""),this);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblNr);
    layout->addWidget(ledLocation);
    layout->addWidget(ledLabel);
    setLayout(layout);
}

QString axisLine::LabelText(void)
{
    /*static int pos,pos2,ret;
    static QString str2;
    static QString ref("$$"),intermediate;*/
    static QString result,str;
    result=str=ledLabel->text();
    if (activateLaTeXsupport==TRUE)
    {//Latex-Commands-replacements
        complete_LaTeX_to_Grace_Translator(result);
        /*
pos=0;
result=str;
while ((pos=str.indexOf(ref,pos))>=0)
{
pos+=2;//beginning of commands after '$$'
pos2=str.indexOf(ref,pos);
str2=str.mid(pos,pos2-pos);
intermediate=ref+str2+ref;
pos=pos2+2;//beginning of text after closing '$$'
strcpy(dummy,str2.toLatin1());
ret=recursive_replacer(dummy);
result.replace(intermediate,QString(dummy));
}*/
        //cout << "end =#" << str.toLatin1().constData() << "#" << endl;
        //cout << "result =#" << result.toLatin1().constData() << "#" << endl;
    }
    return result;
}

QString axisLine::LocationText(void)
{
    return ledLocation->text();
}

void axisLine::SetTextToMemory(char * &t1,char * &t2)
{
    SetLineEditToMemory(t1,t2,c1,c2,original,ledLabel);
}

void axisLine::SetMemoryToText(char * &t1,char * &t2)
{
    DynSetMemoryToLineEdit(t1,t2,c1,c2,original,true,ledLabel);
}

void axisLine::LabelClicked(void)
{
    if (activateLaTeXsupport==FALSE) return;
    ClickedOnLabel(c1,c2,original,ledLabel);
}

void axisLine::mouseReleaseEvent(QMouseEvent * e)
{
    if (activateLaTeXsupport==FALSE) return;
    e->accept();
    QPoint pos=e->pos();
    if (lblNr->rect().contains(pos)==true) LabelClicked();
}

headLine::headLine(QWidget * parent,int columns):QWidget(parent)
{
    QString dummy;
    char dummy2[128];
    cols=columns;

    empty=new QWidget(this);
    signalMapper=new QSignalMapper(this);

    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(empty);

    for (int i=0;i<6;i++)
    {
        switch (i)
        {
        case 0:
            dummy=QString("X");
            break;
        case 1:
            dummy=QString("Y");
            break;
        default:
            sprintf(dummy2,"Y%d",i-1);
            dummy=QString(dummy2);
            break;
        }
        cmdColHead[i]=new QPushButton(dummy,this);
        connect(cmdColHead[i],SIGNAL(clicked()),signalMapper,SLOT(map()));
        signalMapper->setMapping(cmdColHead[i],i);
        if (i<cols)
        {
            cmdColHead[i]->setVisible(TRUE);
            layout->addWidget(cmdColHead[i]);
        }
        else
            cmdColHead[i]->setVisible(FALSE);
    }
    setLayout(layout);
    connect(signalMapper,SIGNAL(mapped(int)),SLOT(pressed(int)));
}

void headLine::resize(int columns)
{
    if (columns<cols)//the new one has lesser columns than the old one
    {
        for (int i=columns;i<cols;i++)
        {
            cmdColHead[i]->setVisible(FALSE);
            layout->removeWidget(cmdColHead[i]);
        }
    }
    else//more columns
    {
        for (int i=cols;i<columns;i++)
        {
            cmdColHead[i]->setVisible(TRUE);
            layout->addWidget(cmdColHead[i]);
        }
    }
    update();
    parentWidget()->updateGeometry();
    cols=columns;
}

void headLine::pressed(int i)
{
    emit(col_pressed(i));
}

spreadLine::spreadLine(QWidget * parent,int row_nr,int columns):QWidget(parent)
{
    char dummy[128];
    marked=false;
    row=row_nr;
    cols=columns;
    sprintf(dummy,"%d",row);
    cmdSelect=new QPushButton(QString(dummy),this);
    connect(cmdSelect,SIGNAL(clicked()),SLOT(clickSelect()));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(cmdSelect);
    for (int i=0;i<6;i++)
    {
        ledInput[i]=new QLineEdit(QString(""),this);
        if (i<cols)
        {
            ledInput[i]->setVisible(TRUE);
            layout->addWidget(ledInput[i]);
        }
        else
            ledInput[i]->setVisible(FALSE);

    }
    setLayout(layout);
}

spreadLine::~spreadLine()
{
    for (int i=0;i<6;i++)
    {
        if (i<cols)
            layout->removeWidget(ledInput[i]);
        ledInput[i]->disconnect();
        delete ledInput[i];
    }
}

void spreadLine::resize(int columns)
{
    if (cols>columns)//new are fewer columns
    {
        for (int i=columns;i<cols;i++)
        {
            layout->removeWidget(ledInput[i]);
            ledInput[i]->setVisible(FALSE);
        }
    }
    else
    {
        for (int i=cols;i<columns;i++)
        {
            layout->addWidget(ledInput[i]);
            ledInput[i]->setVisible(TRUE);
        }
    }
    cols=columns;
}

void spreadLine::clickSelect(void)
{
    marked=!marked;
    QPalette pal;
    QBrush t1,t2;
    for (int i=0;i<6;i++)
    {
        pal=ledInput[i]->palette();
        t1=pal.text();
        t2=pal.base();
        //exchange forground and background to show selection
        pal.setBrush(QPalette::Active,QPalette::Base,t1);
        pal.setBrush(QPalette::Active,QPalette::Text,t2);
        ledInput[i]->setPalette(pal);
    }
}

spreadSheet::spreadSheet(QWidget * parent,int type,int row_count,int col_count):QWidget(parent)
{
    sheet_type=type;
    rows=row_count;//rows with actual inputs
    cols=col_count;

    layout=new QVBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    generate_layout();

    setLayout(layout);

    /*stdWidth=ledInput[0][0]->width();
stdHeight=ledInput[0][0]->height();*/

}

void spreadSheet::generate_layout(void)
{
    signalMapper=new QSignalMapper(this);
    switch (sheet_type)
    {
    case 0:
        headline=new headLine(this,cols);
        layout->addWidget(headline);
        spreadlines=new spreadLine*[rows];
        for (int i=0;i<rows;i++)
        {
            spreadlines[i]=new spreadLine(this,i,cols);
            layout->addWidget(spreadlines[i]);
        }
    /*if (rows>0)
    {
    headline->empty->setMaximumWidth(spreadlines[0]->cmdSelect->width());
    }*/
        break;
    case 1:
        fitlines=new fitLine*[MAXPARM+1];
        for (int i=0;i<=MAXPARM;i++)
        {
            fitlines[i]=new fitLine(this,i);
            if (i<rows)
            {
                fitlines[i]->setVisible(TRUE);
                layout->addWidget(fitlines[i]);
            }
            else
            {
                fitlines[i]->setVisible(FALSE);
            }
        }
        break;
    case 2:
        axislines=new axisLine*[rows];
        for (int i=0;i<rows;i++)
        {
            axislines[i]=new axisLine(this,i);
            layout->addWidget(axislines[i]);
        }
        break;
    }

    for (int i=0;i<rows;i++)
    {
        ;
        //layout->addWidget();
        //connect(VHeads[i],SIGNAL(clicked()),signalMapper1,SLOT(map()));
        //signalMapper1->setMapping(VHeads[i],i);
    }
    //connect(signalMapper,SIGNAL(mapped(int)),SLOT(clickHHeader(int)));
}

spreadSheet::~spreadSheet()
{

}

void spreadSheet::clickHHeader(int nr)
{
    (void)nr;
/*EditColumn->selColumn->setCurrentIndex(nr);
EditColumn->show();
EditColumn->raise();*/
}

void spreadSheet::clickVHeader(int nr)
{
    (void)nr;
    /*marked[nr]=!marked[nr];
QPalette pal;
QBrush t1,t2;
    for (int i=0;i<cols;i++)
    {
    pal=ledInput[nr][i]->palette();
    t1=pal.text();
    t2=pal.base();
    //exchange forground and background to show selection
    pal.setBrush(QPalette::Active,QPalette::Base,t1);
    pal.setBrush(QPalette::Active,QPalette::Text,t2);
    ledInput[nr][i]->setPalette(pal);
    }*/
}

void spreadSheet::entryChanged(void)
{
    emit(changed());
}

void spreadSheet::resize(int row,int col)
{

    /*if (rows>0 && !sheet_type)
{
for (int i=0;i<6;i++)
headline->cmdColHead[i]->setMinimumSize(spreadlines[0]->ledInput[i]->width(),spreadlines[0]->ledInput[i]->height());
}*/

    switch (sheet_type)
    {
    case 0:

        setGeometry(0,0,width(),(row+1)*25);

        headline->resize(col);

        for (int i=0;i<rows;i++)
        {
            layout->removeWidget(spreadlines[i]);
            delete spreadlines[i];
        }
        delete[] spreadlines;

        spreadlines=new spreadLine*[row];
        for (int i=0;i<row;i++)
        {
            spreadlines[i]=new spreadLine(this,i,col);
            layout->addWidget(spreadlines[i]);
        }
        rows=row;
        cols=col;

        break;
    case 1:
        if (row>rows)
        {
            for (int i=rows;i<row;i++)
            {
                layout->addWidget(fitlines[i]);
                fitlines[i]->setVisible(TRUE);
            }
        }
        else
        {
            for (int i=row;i<rows;i++)
            {
                fitlines[i]->setVisible(FALSE);
                layout->removeWidget(fitlines[i]);
            }
        }
        rows=row;
        cols=col;
        break;
    case 2:

        break;
    }

    /*
setGeometry(0,0,(col+1)*stdWidth,(row+1)*stdHeight);
///delete current Elements
layout->removeWidget(empty);
delete empty;
delete[] marked;
for (int i=0;i<rows;i++)
{
layout->removeWidget(VHeads[i]);
VHeads[i]->disconnect();
delete VHeads[i];
    for (int j=0;j<cols;j++)
    {
    layout->removeWidget(ledInput[i][j]);
    ledInput[i][j]->disconnect();
    delete ledInput[i][j];
    }
delete[] ledInput[i];
}
delete[] ledInput;
delete[] VHeads;
for (int i=0;i<6;i++)
{
layout->removeWidget(HHeads[i]);
HHeads[i]->disconnect();
delete HHeads[i];
}
delete[] HHeads;

signalMapper1->disconnect();
signalMapper2->disconnect();

rows=row;
cols=col;

generate_layout();
*/
    update();
    parentWidget()->updateGeometry();
}

stdSlider::stdSlider(QWidget * parent,QString label,int min,int max,double factor,int type):QWidget(parent)
{
    QRect rec=stdFontMetrics->boundingRect(QString("100"));
    ret_time=new QTime();
    ret_time->start();
    textHeight=rec.height();
    slideType=type;
    ScalingFactor=factor;
    if (textHeight<16) textHeight=16;
    //setMinimumSize(QSize(180,textHeight*3));
    //Indicator=new QLabel("0",this);
    Indicator=new QLineEdit("0",this);
    Indicator->setGeometry(0,0,38,textHeight);//0,0,32,20
    sldSlider=new QSlider(Qt::Horizontal,this);
    sldSlider->setRange(min,max);
    sldSlider->setGeometry(Indicator->x(),Indicator->y()+Indicator->height(),200,textHeight);//20
    connect(sldSlider,SIGNAL(sliderPressed()),SLOT(SliderPressed()));
    connect(sldSlider,SIGNAL(sliderReleased()),SLOT(SliderReleased()));
    connect(sldSlider,SIGNAL(sliderMoved(int)),SLOT(SliderMoved(int)));
    connect(sldSlider,SIGNAL(valueChanged(int)),SLOT(changed(int)));

    lblText=new QLabel(label,this);
    lblText->move(Indicator->x(),sldSlider->y()+sldSlider->height());//,150,20);
    lblText->setGeometry(Indicator->x(),sldSlider->y()+sldSlider->height(),sldSlider->width(),textHeight);//20
    //setMinimumSize(60,3*textHeight);
    //setMinimumSize(20,20);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::MinimumExpanding);
    /// BEGIN LineEdit decorations
    Indicator->setFrame(false);
    Indicator->setValidator(new QDoubleValidator(Indicator));
    QPalette pal1=lblText->palette();
    pal1.setBrush(QPalette::Base,Qt::NoBrush);
    Indicator->setPalette(pal1);
    /// END LineEdit decorations
    connect(Indicator,SIGNAL(returnPressed()),SLOT(Indicator_Enter_Pressed()));
    connect(Indicator,SIGNAL(editingFinished()),SLOT(Indicator_Finished()));
}

QSize stdSlider::sizeHint() const
{
int h,w;
    QFontMetrics fm(Indicator->font());
    QSize si=fm.boundingRect(QString("100g")).size();
    w=4*si.width();
    if (lblText->text().length()<1)
    h=2.5*(si.height()+fm.descent());
    else
    h=3.5*(si.height()+fm.descent());
    //return QSize(60,textHeight*3);
    //return QSize(20,84);
//qDebug() << "Slider: sizeHint=" << w << " x " << h;
    return QSize(w,h);
}

QSize stdSlider::minimumSizeHint() const
{
int h,w;
    QFontMetrics fm(Indicator->font());
    QSize si=fm.boundingRect(QString("100g")).size();
/*
    qDebug() << "descent=" << fm.descent() << " ascent=" << fm.ascent();
    qDebug() << "Indicator: 100g: size=" << si;
    qDebug() << "Indicator: 1000: size=" << fm.boundingRect(QString("1000")).size();
    fm=QFontMetrics(lblText->font());
    qDebug() << "Text: 100g: size=" << fm.boundingRect(QString("100g")).size();
    qDebug() << "Text: 1000: size=" << fm.boundingRect(QString("1000")).size();
*/
    w=3*si.width();
    if (lblText->text().length()<1)
    h=2*(si.height()+fm.descent());
    else
    h=3*(si.height()+fm.descent());
    //return QSize(60,textHeight*3);
    //return QSize(20,84);
    if (w<120) w=120;
    if (h<70) h=70;
//qDebug() << "Slider: MinSizeHint=" << w << " x " << h;
    return QSize(w,h);
}

void stdSlider::resizeEvent( QResizeEvent * e)
{
static int width,height,lblTextHeight,offset;
static QFont fo;
static QString indText;
    offset=2;
e->accept();
    width=e->size().width();
    height=e->size().height();

    if (lblText->text().length()<1)
    {
    lblText->setVisible(false);
    lblTextHeight=0;
    //qDebug() << "INVISIBLE";
    }
    else
    {
    lblText->setVisible(true);
    lblTextHeight=1;
    //qDebug() << "VISIBLE !!";
    }
//qDebug() << "A RESIZE SLIDER: " << lblText->text() << " (len=" << lblText->text().length() << ") w=" << width << " h=" << height << " Value=" << Indicator->text() << " visible=" << lblText->isVisible();
    fo=qApp->font();
    textHeight=(height-(1+lblTextHeight)*offset)/(2+lblTextHeight);
    if (fo.pixelSize()>(int)rint_2(0.8*textHeight))
    {
    if ((int)rint_2(0.8*textHeight)<14) fo.setPixelSize(14);
    else fo.setPixelSize((int)rint_2(0.8*textHeight));
    lblText->setFont(fo);
    Indicator->setFont(fo);
    }
    QFontMetrics fm(Indicator->font());
    //QSize si=fm.boundingRect(Indicator->text()).size();
    QSize si=fm.boundingRect(QString("100g")).size();
    textHeight=si.height()+fm.descent();
    /*if (height<(2+lblTextHeight)*textHeight)
    {
        fo.setPixelSize(fo.pixelSize()-2);
        textHeight-=2;
        lblText->setFont(fo);
        Indicator->setFont(fo);
    }
    else if (height<(2+lblTextHeight)*textHeight && fo.pixelSize()<14)
    {
        fo.setPixelSize(14);
        textHeight-=2;
        lblText->setFont(fo);
        Indicator->setFont(fo);
    }
    Indicator->setGeometry(Indicator->x(),0,Indicator->width(),textHeight);*/
    Indicator->setGeometry(Indicator->x(),0,si.width()+2,si.height()+2);
    sldSlider->setGeometry(0,textHeight+offset,width,height-(1+lblTextHeight)*(textHeight+offset));
        if (lblTextHeight)
        {
        lblText->setGeometry(0,height-textHeight-offset,lblText->width(),textHeight);
        //qDebug() << "lblText: (" << lblText->x() << "|" << lblText->y() << ") w=" << lblText->width() << " h=" << lblText->height();
        }
    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
//qDebug() << "Indicator: (" << Indicator->x() << "|" << Indicator->y() << ") w=" << Indicator->width() << " h=" << Indicator->height();
//qDebug() << "sldSlider: (" << sldSlider->x() << "|" << sldSlider->y() << ") w=" << sldSlider->width() << " h=" << sldSlider->height();
//qDebug() << "B RESIZE SLIDER: w=" << this->width() << " h=" << this->height();
    QWidget::resizeEvent(e);
}

void stdSlider::changed(int i)
{
static QString indText;
    (void)i;
    indText=indicatorText();
    slideIndicator();
    Indicator->setText(indText);
    emit(valueChanged(i));
    if (slideType!=SLIDE_LINEAR)
    sldSlider->setFocus();
}

void stdSlider::SliderMoved(int i)
{
blockSignals(true);
    //(void)i;
    changed(i);
    //cout << "moved " << i << " old=" << old_value << endl;
blockSignals(false);
}

void stdSlider::SliderReleased(void)
{
blockSignals(true);
    slider_status=3;
    changed(value());
    //cout << "released old=" << old_value << " new=" << value() << endl;
    old_value=value();
blockSignals(false);
}

void stdSlider::SliderPressed(void)
{
blockSignals(true);
    //cout << "Pressed old=" << old_value << endl;
    slider_status=1;
    sldSlider->setFocus();
    changed(value());
blockSignals(false);
}

int stdSlider::value(void)
{
    return sldSlider->value();
}

void stdSlider::setValue(int i)
{
    old_value=i;
    sldSlider->setValue(i);
    changed(i);
}

void stdSlider::redisplay(void)
{
    setValue(value());
}

QString stdSlider::indicatorText(void)
{
    static char dummy[16];
    static int i;
    i=sldSlider->value();
    if (slideType==SLIDE_LINEAR)
    {
        if (ScalingFactor==1.0)
            sprintf(dummy,"%d",i);
        else
            sprintf(dummy,"%g",i*ScalingFactor);
    }
    else//logarithmic
    {
        if (pow(10.0,i*ScalingFactor)<1.0)
        sprintf(dummy,"%2.02f",pow(10.0,i*ScalingFactor));//"%#2.2g"
        else
        sprintf(dummy,"%2.01f",pow(10.0,i*ScalingFactor));//"%#2.2g"
    }
    SetDecimalSeparatorToUserValue(dummy);
    return QString(dummy);
}

void stdSlider::slideIndicator(void)
{
    static int pos;
    pos=sldSlider->x()+(sldSlider->width()-Indicator->width())*(sldSlider->value()-sldSlider->minimum())/(sldSlider->maximum()-sldSlider->minimum());
    Indicator->move(pos,Indicator->y());
}

void stdSlider::Indicator_Enter_Pressed(void)
{
    ret_time->restart();
}

void stdSlider::Indicator_Finished(void)
{
    int time_since_enter=ret_time->restart();
    double val=Indicator->text().toDouble();
//cout << "manual change: old_value=" << old_value << endl;
    if (time_since_enter<100)//less then 100ms since last Return pressed
    {
        //cout << "Return Pressed (" << val << ")" << endl;
        if (slideType==SLIDE_LINEAR)
        {
            setValue((int)val);
        }
        else
        {
            setValue((int)(log10(val)/ScalingFactor));
        }
    }
    else
    {
        //cout << "Indicator Finished without Return (" << val << ")" << endl;
        Indicator->setText(indicatorText());
    }
//cout << "manual change: cur_val=" << value() << endl;
}

FontSelector::FontSelector(QWidget * parent):QWidget(parent)
{
    font=qApp->font();
    cmdSelFont=NULL;
    lblText=NULL;
    //if (useQtFonts==false)
    lblText=new QLabel(tr("Font:"));
    //else
    //{
    cmdSelFont=new QPushButton(tr("Font:"),this);
    connect(cmdSelFont,SIGNAL(clicked()),SLOT(selectNewFont()));
    displayFont();
    //}
    cmbFontSelect=new QComboBox();
    for (unsigned int i=0;i<number_of_fonts();i++)
        cmbFontSelect->addItem(get_fontalias((int)i));
    connect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
    cmbFontSelect->setCurrentIndex(DefaultFont);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    //if (useQtFonts==false)
    layout->addWidget(lblText);
    //else
    layout->addWidget(cmdSelFont);
    layout->addWidget(cmbFontSelect);
    setLayout(layout);
    add_FontSelector(this);
    updateFonts(false);
    updateAppearance(useQtFonts);
}

void FontSelector::updateAppearance(bool QtIsNew)
{
    if (QtIsNew==true)//Qt-font-selector-button
    {
        lblText->setVisible(false);
        cmdSelFont->setVisible(true);
    }
    else//we switch back to Grace-standard
    {
        cmdSelFont->setVisible(false);
        lblText->setVisible(true);
    }
}

void FontSelector::setLabelText(QString s)
{
    if (useQtFonts==false)
        lblText->setText(s);
    else
        cmdSelFont->setText(s);
}

void FontSelector::updateFonts(bool preserve)
{
    disconnect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
    int old_index=cmbFontSelect->currentIndex();
    QString description;
    cmbFontSelect->clear();
    if (useQtFonts==false)
    {
        for (unsigned int i=0;i<number_of_fonts();i++)
            cmbFontSelect->addItem(get_fontalias((int)i));
    }
    else//useQtFonts=true
    {
        for (int i=0;i<stdFontList.length();i++)
        {
            description=stdFontList.at(i).family();
            if (stdFontList.at(i).italic()==true)
                description=description + tr("-italic");
            if (stdFontList.at(i).bold()==true)
                description=description + tr("-bold");
            cmbFontSelect->addItem(description);
        }
    }
    if (preserve==true)
        cmbFontSelect->setCurrentIndex(old_index);
    else
        cmbFontSelect->setCurrentIndex(DefaultFont);

    font=getFontFromDatabase(cmbFontSelect->currentIndex());

    connect(cmbFontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(changed(int)));
}

void FontSelector::selectNewFont(void)
{
    int index;
    bool ok;
    font=QFontDialog::getFont(&ok,font,this);
    if (ok==true)//really a new font!
    {
        index=indexOfFontInDatabase(font);//does it exist?
        if (index<0)//no --> new font in database
        {
            index=addFontToDatabase(font);
            update_font_selectors(false);//update all Font selectors (just fonts, not apperance)
            changed(index);
        }
        cmbFontSelect->setCurrentIndex(index);
        displayFont();
    }
}

void FontSelector::displayFont(void)
{
    static QFont dFont;
    if (cmdSelFont!=NULL)
    {
        dFont=font;
        if (qApp->font().pixelSize()<2)
            dFont.setPixelSize(10);
        else
            dFont.setPixelSize(qApp->font().pixelSize());
        cmdSelFont->setFont(dFont);
        cmdSelFont->setText(cmdSelFont->text());
        //cmdSelFont->setText(font.family());
        //cout << "font=#" << font.toString().toLatin1().constData() << "#" << endl;
    }
}

void FontSelector::changed(int i)
{
    font=getFontFromDatabase(i);
    displayFont();
    emit(currentIndexChanged(i));
}

int FontSelector::currentIndex(void)
{
    /*if (useQtFonts==true)
    return indexOfFontInDatabase(font);
    else*/
    return cmbFontSelect->currentIndex();
}

void FontSelector::setCurrentIndex(int i)
{
    font=getFontFromDatabase(i);
    displayFont();
    cmbFontSelect->setCurrentIndex(i);
}

QFont FontSelector::getCurrentFont(void)
{
    return font;
}

void FontSelector::setCurrentFont(QFont & f)
{
    font = f;
    displayFont();
    int index=indexOfFontInDatabase(f);
    cmbFontSelect->setCurrentIndex(index);
}

PositionSelector::PositionSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Position in:"));
    cmbPositionSelect=new QComboBox();
    cmbPositionSelect->addItem(tr("World coordinates"));
    cmbPositionSelect->addItem(tr("Viewport coordinates"));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbPositionSelect);
    setLayout(layout);
}

int PositionSelector::currentIndex(void)
{
    return cmbPositionSelect->currentIndex();
}

int PositionSelector::currentValue(void)
{
    return cmbPositionSelect->currentIndex()==0?COORD_WORLD:COORD_VIEW;
}

void PositionSelector::setCurrentIndex(int i)
{
    cmbPositionSelect->setCurrentIndex(i);
}

JustificationSelector::JustificationSelector(QWidget * parent):QWidget(parent)
{
    just_names=new QString*[12];

    just_names[0]=new QString("lm");
    values[0]=JUST_LEFT | JUST_BLINE;
    just_names[1]=new QString("cm");
    values[1]=JUST_CENTER | JUST_BLINE;
    just_names[2]=new QString("rm");
    values[2]=JUST_RIGHT | JUST_BLINE;
    just_names[3]=new QString("lb");
    values[3]=JUST_LEFT | JUST_BOTTOM;
    just_names[4]=new QString("cb");
    values[4]=JUST_CENTER | JUST_BOTTOM;
    just_names[5]=new QString("rb");
    values[5]=JUST_RIGHT | JUST_BOTTOM;
    just_names[6]=new QString("lm");
    values[6]=JUST_LEFT | JUST_MIDDLE;
    just_names[7]=new QString("cm");
    values[7]=JUST_CENTER | JUST_MIDDLE;
    just_names[8]=new QString("rm");
    values[8]=JUST_RIGHT | JUST_MIDDLE;
    just_names[9]=new QString("lt");
    values[9]=JUST_LEFT | JUST_TOP;
    just_names[10]=new QString("ct");
    values[10]=JUST_CENTER | JUST_TOP;
    just_names[11]=new QString("rt");
    values[11]=JUST_RIGHT | JUST_TOP;

    cmbJustSelect=new newCombo(3,4,3,Qt_justifications,just_names,false,this);
    lblText=new QLabel(tr("Justification:"));

    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbJustSelect);
    setLayout(layout);
}

int JustificationSelector::currentIndex(void)
{
    return values[cmbJustSelect->currentIndex()];
}

void JustificationSelector::setCurrentIndex(int i)
{
    int index=0;
    for (int j=0;j<12;j++)
    {
        if (values[j]==i)
        {
            index=j;
            break;
        }
    }
    cmbJustSelect->setCurrentIndex(index);
}

stdButtonGroup::stdButtonGroup(QWidget * parent,bool appl,bool acc,bool help):QWidget(parent)
{
    showAccept=acc;
    showApply=appl;
    showHelp=help;

    cmdAccept=new QPushButton(tr("Accept"));
    cmdApply=new QPushButton(tr("Apply"));
    cmdClose=new QPushButton(tr("Close"));
    cmdHelp=new QPushButton(tr("Help"));

    layout=new QHBoxLayout;
    layout->setMargin(2);
    if (showApply==true)
        layout->addWidget(cmdApply);
    else
        cmdApply->hide();
    if (showAccept==true)
        layout->addWidget(cmdAccept);
    else
        cmdAccept->hide();

    layout->addWidget(cmdClose);

    if (showHelp==true)
        layout->addWidget(cmdHelp);
    else
        cmdHelp->hide();
    setLayout(layout);
}

ColorSelector::ColorSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Color:"));
    prevent_from_update=false;
/*
cmbColorSelect=new QComboBox();
for (int i=0;i<16;i++)
cmbColorSelect->addItem(*ColorIcons[i],"");
cmbColorSelect->setIconSize(QSize(82,16));
*/
    int * real_colors=new int[4];
    int aux_cols;
    int map_entries=get_main_color_indices(&real_colors,&aux_cols);
    int rows=int(sqrt(map_entries*1.0));
    int cols=rows;
    int last_col=rows;
    if (rows*cols<map_entries)
    {
        last_col=map_entries-rows*cols;
        cols+=1;
        while (last_col>rows)
        {
            cols+=1;
            last_col-=rows;
        }
    }
    cmbColorSelect=new newCombo(rows,cols,last_col,ColorPixmaps,ColorNames,true,this,true);
    connect(cmbColorSelect,SIGNAL(current_Index_Changed(int)),this,SLOT(panelIndexChanged(int)));
    connect(cmbColorSelect->panels,SIGNAL(newAlpha(int)),this,SLOT(alphaValueChanged2(int)));
    connect(cmbColorSelect->panels->alphaSlider,SIGNAL(alphaChanged(int)),this,SLOT(alphaValueChanged2(int)));
    delete[] real_colors;
    alphaSelector=new alphaFrame(this,true);
    connect(alphaSelector,SIGNAL(valueChanged(int)),this,SLOT(alphaValueChanged(int)));
    connect(cmbColorSelect->panels->alphaSlider,SIGNAL(alphaChanged(int)),alphaSelector,SLOT(setAlpha(int)));
    layout=new QHBoxLayout;
    layout->setAlignment(Qt::AlignVCenter);
    layout->setMargin(STD_MARGIN);
    layout->setSpacing(STD_SPACING);
    layout->addWidget(lblText);
    layout->addWidget(cmbColorSelect);
    layout->addWidget(alphaSelector);
    setLayout(layout);

    add_ColorSelector(this);

bool vis_extern=(show_transparency_selector==1?true:false);
bool vis_intern=(show_transparency_selector==2?true:false);
alphaSelector->setVisible(vis_extern);
cmbColorSelect->panels->alphaSlider->setVisible(vis_intern);
}

void ColorSelector::update_color_panels(void)
{
    updateColorIcons(number_of_colors(),ColorPixmaps,ColorNames);
}

void ColorSelector::panelIndexChanged(int i)
{
//qDebug() << "color Changed in Panels i=" << i;
    emit(currentIndexChanged(i));
}

void ColorSelector::alphaValueChanged(int i)
{
    cmbColorSelect->panels->alpha=i;
    cmbColorSelect->panels->alphaSlider->slider->setValue(i);
    emit(alphaChanged(i));
}

void ColorSelector::alphaValueChanged2(int i)
{
//qDebug() << "alpha Changed in Panels i=" << i;
    alphaSelector->setAlpha(i);
}

int ColorSelector::currentIndex(void)
{
    return cmbColorSelect->currentIndex();
}

int ColorSelector::alpha(void)
{
    return alphaSelector->alpha;
}

void ColorSelector::setCurrentIndex(int i)
{
    cmbColorSelect->setCurrentIndex(i);
}

void ColorSelector::setAlpha(int i)
{
    if (i<0 || i>255)
    {
    alphaSelector->setAlpha_extern(255);
    cmbColorSelect->panels->setAlpha_extern(255);
    }
    else
    {
    alphaSelector->setAlpha_extern(i);
    cmbColorSelect->panels->setAlpha_extern(i);
    }
}

void ColorSelector::updateColorIcons(int nr_of_cols,QPixmap ** ColorPixmaps,QString ** ColorNames)
{
    int rows=int(sqrt(nr_of_cols*1.0));
    int cols=rows;
    int last_col=rows;
    if (rows*cols<nr_of_cols)
    {
        last_col=nr_of_cols-rows*cols;
        cols+=1;
        while (last_col>rows)
        {
            cols+=1;
            last_col-=rows;
        }
    }
    cmbColorSelect->reinitializePanels(rows,cols,last_col,ColorPixmaps,ColorNames,cmbColorSelect->text_only,(cmbColorSelect->panels->alphaSlider!=NULL?true:false));
}

StdSelector::StdSelector(QWidget * parent)
{
    int number=2;
    QString * entr=new QString[2];
    entr[0]=QString("");
    entr[1]=QString("");
    QString label("");
    StdSelector(parent,label,number,entr);
    entryIcons=NULL;
}

StdSelector::StdSelector(QWidget * parent,QString label,int number,QString * entr):QWidget(parent)
{
    lblText=new QLabel(label);
    cmbSelect=new QComboBox();
    entryIcons=NULL;
if (number<1 || entr==NULL)
{
    number_of_entries=2;
    entries=new QString[number_of_entries];
    entryValues=new int[number_of_entries];
    for (int i=0;i<number_of_entries;i++)
    {
        entries[i]=QString("");
        entryValues[i]=i;
        cmbSelect->addItem(entries[i]);
    }
}
else
{
    number_of_entries=number;
    entries=new QString[number];
    entryValues=new int[number];
    for (int i=0;i<number;i++)
    {
        entries[i]=entr[i];
        entryValues[i]=i;
        cmbSelect->addItem(entr[i]);
    }
}
    connect(cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbSelect);
    setLayout(layout);
}

StdSelector::~StdSelector()
{
    delete[] entries;
    delete[] entryValues;
    if (entryIcons!=NULL) delete[] entryIcons;
}

void StdSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

void StdSelector::setValues(int * values)
{
    for (int i=0;i<number_of_entries;i++)
    {
        entryValues[i]=values[i];
    }
}

void StdSelector::setNewEntries(int number,QString * entr)
{
    int * values=new int[number];
    for (int i=0;i<number;i++)
        values[i]=i;
    setNewEntries(number,entr,values);
    delete[] values;
}

void StdSelector::setNewEntries(int number,QString * entr,int * values)
{
    delete[] entries;
    delete[] entryValues;
    number_of_entries=number;
    entries=new QString[number];
    entryValues=new int[number];
    for (int i=0;i<number;i++)
    {
        entries[i]=entr[i];
        entryValues[i]=values[i];
    }
    redisplayItems();
}

void StdSelector::setIcons(QIcon * icons)
{
    if (entryIcons!=NULL) delete[] entryIcons;
    if (icons==NULL)
    {
    entryIcons=NULL;
    return;
    }
    entryIcons=new QIcon[number_of_entries+1];
    for (int i=0;i<number_of_entries;i++) entryIcons[i]=icons[i];
    redisplayItems();
}

void StdSelector::redisplayItems(void)
{
    /*int cur_sel=currentIndex();
cout << "cur_sel=" << cur_sel << endl;
    this->blockSignals(true);*/
    cmbSelect->clear();
    if (entryIcons==NULL || icon_preferences==0)
    {
        for (int i=0;i<number_of_entries;i++)//text only
        {
        cmbSelect->addItem(entries[i]);
        }
    }
    else if (icon_preferences==1)//text and icons
    {
        for (int i=0;i<number_of_entries;i++)
        {
        cmbSelect->addItem(entryIcons[i],entries[i]);
        }
    }
    else//icons only
    {
        for (int i=0;i<number_of_entries;i++)
        {
        cmbSelect->addItem(entryIcons[i],QString(""));
        }
    }
    /*this->blockSignals(false);
    setCurrentIndex(cur_sel);*/
}

int StdSelector::currentIndex(void)
{
    return cmbSelect->currentIndex();
}

int StdSelector::currentValue(void)
{
    return entryValues[cmbSelect->currentIndex()];
}

void StdSelector::setCurrentIndex(int i)
{
    if (i>=0 && i<cmbSelect->count())
        cmbSelect->setCurrentIndex(i);
    else
        cmbSelect->setCurrentIndex(0);
}

void StdSelector::setCurrentValue(int i)
{
int index=indexOfValueInList(i);
    if (index<0)
    cmbSelect->setCurrentIndex(0);
    else
    cmbSelect->setCurrentIndex(index);
}

int StdSelector::indexOfValueInList(int v)//returns the index of the value v in the entryValues-list (-1 if not found
{
int index=-1;
    for (int j=0;j<number_of_entries;j++)
    {
        if (v==entryValues[j])
        {
            index=j;
            break;
        }
    }
return index;
}

bool StdSelector::valueIsInList(int v)//returns true, if v is in the list of entryValues
{
return indexOfValueInList(v)==-1?FALSE:TRUE;
}

LineStyleSelector::LineStyleSelector(QWidget * parent):QWidget(parent)
{
    add_Line_Style_Selector(this);
    lblText=new QLabel(tr("Line style:"));
    /*cmbStyleSelect=new QComboBox();
for (int i=0;i<MAXLINESTYLES;i++)
cmbStyleSelect->addItem(*LineIcons[i],"");
cmbStyleSelect->setIconSize(QSize(82,22));*/

    LineNames=new QString*[nr_of_current_linestyles];
    char dummy[48];
    for (int i=0;i<nr_of_current_linestyles;i++)
    {
        sprintf(dummy,"%d",i);
        LineNames[i]=new QString(dummy);
    }
    cmbStyleSelect=new newCombo(nr_of_current_linestyles,1,nr_of_current_linestyles,LinePixmaps,LineNames,false,this);
    connect(cmbStyleSelect,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbStyleSelect);
    setLayout(layout);
}

void LineStyleSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

int LineStyleSelector::currentIndex(void)
{
    return cmbStyleSelect->currentIndex();
}

void LineStyleSelector::setCurrentIndex(int i)
{
    cmbStyleSelect->setCurrentIndex(i);
}

FillPatternSelector::FillPatternSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Fill pattern:"),this);
    /*
cmbFillPattern=new QComboBox(this);
cmbFillPattern->addItem(tr("None"));
for (int i=1;i<MAXPATTERNS;i++)
cmbFillPattern->addItem(QIcon(patterns[i]->copy(patterns[i]->rect())),"");
*/
    char dummy[48];
    PatternNames=new QString*[MAXPATTERNS];
    sprintf(dummy,"None");
    PatternNames[0]=new QString(dummy);
    for (int i=1;i<MAXPATTERNS;i++)
    {
        sprintf(dummy,"%d",i);
        PatternNames[i]=new QString(dummy);
    }
    cmbFillPattern=new newCombo(8,4,8,PatternPixmaps,PatternNames,false,this);
    connect(cmbFillPattern,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbFillPattern);
    setLayout(layout);
}

void FillPatternSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

int FillPatternSelector::currentIndex(void)
{
    return cmbFillPattern->currentIndex();
}

void FillPatternSelector::setCurrentIndex(int i)
{
    cmbFillPattern->setCurrentIndex(i);
}

OrderSelector::OrderSelector(QWidget * parent):QWidget(parent)
{
    OrderNames=new QString*[4*2];
    OrderNames[0]=new QString("hv_lr");
    OrderNames[1]=new QString("hv_lr");
    OrderNames[2]=new QString("hv_rl");
    OrderNames[3]=new QString("hv_rl");
    OrderNames[4]=new QString("vh_lr");
    OrderNames[5]=new QString("vh_lr");
    OrderNames[6]=new QString("vh_rl");
    OrderNames[7]=new QString("vh_rl");
    lblText=new QLabel(tr("Order:"),this);
    cmbOrderPattern=new newCombo(4,2,4,Qt_matrixOrder,OrderNames,false,this);
    connect(cmbOrderPattern,SIGNAL(current_Index_Changed(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbOrderPattern);
    setLayout(layout);
}

void OrderSelector::changed(int i)
{
    emit(currentIndexChanged(i));
}

int OrderSelector::currentIndex(void)
{
    return cmbOrderPattern->currentIndex();
}

void OrderSelector::setCurrentIndex(int i)
{
    cmbOrderPattern->setCurrentIndex(i);
}

LineWidthSelector::LineWidthSelector(QWidget * parent):QWidget(parent)
{
    lblText=new QLabel(tr("Line width:"));
    spnLineWidth=new QDoubleSpinBox(this);
    spnLineWidth->setRange(0.0,MAX_LINEWIDTH);
    spnLineWidth->setDecimals(1);
    spnLineWidth->setSingleStep(0.5);
    connect(spnLineWidth,SIGNAL(valueChanged(double)),SLOT(changed(double)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(spnLineWidth);
    setLayout(layout);
}

void LineWidthSelector::setText(QString txt)
{
    lblText->setText(txt);
}

void LineWidthSelector::setRange(double a,double b)
{
    spnLineWidth->setRange(a,b);
}

void LineWidthSelector::setSingleStep(double a)
{
    spnLineWidth->setSingleStep(a);
}

void LineWidthSelector::setDecimals(int d)
{
    spnLineWidth->setDecimals(d);
}

void LineWidthSelector::changed(double i)
{
    emit(currentValueChanged(i));
}

double LineWidthSelector::value(void)
{
    return spnLineWidth->value();
}

void LineWidthSelector::setValue(double i)
{
    spnLineWidth->setValue(i);
}

stdIntSelector::stdIntSelector(QWidget * parent,QString label,int min,int max):QWidget(parent)
{
    lblText=new QLabel(label,this);
    spnInt=new QSpinBox(this);
    spnInt->setRange(min,max);
    connect(spnInt,SIGNAL(valueChanged(int)),SLOT(changed(int)));
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(spnInt);
    setLayout(layout);
}

void stdIntSelector::changed(int i)
{
    emit(currentValueChanged(i));
}

int stdIntSelector::value(void)
{
    return spnInt->value();
}

void stdIntSelector::setValue(int i)
{
    spnInt->setValue(i);
}

stdLineEdit::stdLineEdit(QWidget * parent,QString label,bool accLaTex):QWidget(parent)
{
    c1=c2=NULL;
    displayStd=true;
    acceptLaTex=accLaTex;
    lblText=new QLabel(label,this);
    lenText=new QLineEdit("0",this);
    connect(lenText,SIGNAL(returnPressed()),this,SLOT(ContentChanged()));
    layout=new QHBoxLayout();
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(lenText);
    setLayout(layout);
    QPalette pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    if (acceptLaTex==true)
        add_LaTeX_Line(this);
    setAcceptDrops(false);
}

void stdLineEdit::ContentChanged(void)
{
    emit(changed());
}

QString stdLineEdit::text(void)
{
    if (useQtFonts)
        return FileCodec->fromUnicode(lenText->text());
    else
        return lenText->text();
    /*///Deactivated LaTeX-to-Grace-conversion
static int pos,pos2,ret;
static QString str,str2;
static QString ref("$$");
static QString result,intermediate;
result=str=lenText->text();
    if (acceptLaTex==true && activateLaTeXsupport==true)
    {//Latex-Commands-replacements
        cout << "I got used!" << endl;
    complete_LaTeX_to_Grace_Translator(result);
    pos=0;
        while ((pos=str.indexOf(ref,pos))>=0)
        {
        pos+=2;//beginning of commands after '$$'
        pos2=str.indexOf(ref,pos);
        str2=str.mid(pos,pos2-pos);
        intermediate=ref+str2+ref;
        pos=pos2+2;//beginning of text after closing '$$'
        strcpy(dummy,str2.toLatin1());
        ret=recursive_replacer(dummy);
        result.replace(intermediate,QString(dummy));
        }
    //cout << "end =#" << str.toLatin1().constData() << "#" << endl;
    //cout << "result =#" << result.toLatin1().constData() << "#" << endl;
    }
return result;
*/
}

void stdLineEdit::setText(QString text)
{
    displayStd=true;
    c1=c2=NULL;
    if (useQtFonts)
    {
        if (FileCodec==NULL) FileCodec=QTextCodec::codecForLocale();
        lenText->setText(FileCodec->toUnicode(text.toLatin1().constData()));
    }
    else
    {
        //lenText->setText(text);
        if (FileCodec==NULL) FileCodec=QTextCodec::codecForLocale();
        lenText->setText(FileCodec->toUnicode(text.toLatin1().constData()));
    }

}

void stdLineEdit::mouseReleaseEvent(QMouseEvent * e)
{
    if (acceptLaTex==false) return;
    e->accept();
    QPoint pos=e->pos();
    if (lblText->rect().contains(pos)==true) clickedOnLabel();
}

void stdLineEdit::clickedOnLabel(void)
{
    if (acceptLaTex==false || activateLaTeXsupport==FALSE) return;
    ClickedOnLabel(c1,c2,displayStd,lenText);
    /*
if (c1==NULL || c2==NULL) return;
displayStd=!displayStd;
QPalette pal=lenText->palette();
if (displayStd)//Std is the original user input (probably in LaTeX-format)
{
lenText->setText(QString(c2));
pal.setColor(QPalette::Text,*stdTextColor);
}
else
{
lenText->setText(QString(c1));//converted input (Grace-format)
pal.setColor(QPalette::Text,Qt::red);
}
lenText->setPalette(pal);*/
}

void stdLineEdit::SetTextToMemory(char * t1,char * t2)
{
    SetLineEditToMemory(t1,t2,c1,c2,displayStd,lenText);
    /*c1=t1;
c2=t2;
displayStd=true;
QPalette pal=lenText->palette();
pal.setColor(QPalette::Text,*stdTextColor);
if (t2!=NULL)
lenText->setText(QString(t2));
else
lenText->setText("");
lenText->setPalette(pal);*/
}

void appendTextToLegendString(int gno,int sno,QString text)
{
    if (is_set_active(gno,sno)==false) return;
    QString old_text;
    plotarr * p=g[gno].p+sno;
    old_text=QString::fromUtf8(p->orig_lstr);//original is in UTF8
    old_text+=text;//append text
    strcpy(p->orig_lstr,old_text.toUtf8().constData());
    if (activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(old_text);
    generate_string_Qt_aware(p->lstr,old_text);//t1 is the Grace-version of the text
}

void replace_directory_separators(char * filename)
{
    char sep=QDir::separator().toLatin1();
    int len=strlen(filename);
    for (int i=0;i<len;i++)
    {
        if (filename[i]=='/' || filename[i]=='\\')
            filename[i]=sep;
    }
}

void stdLineEdit::SetMemoryToText(char * t1,char * t2)
{
    static QPalette pal;
    QString text=lenText->text();
    displayStd=true;
    strcpy(t2,text.toUtf8().constData());//t2 is always the original - internally always in UTF8
    if (acceptLaTex==true && activateLaTeXsupport==TRUE)
        complete_LaTeX_to_Grace_Translator(text);
    generate_string_Qt_aware(t1,text);//t1 is the Grace-version of the text
    c1=t1;
    c2=t2;
    pal=lenText->palette();
    if (stdTextColor==NULL)
        stdTextColor=new QColor(pal.text().color());
    pal.setColor(QPalette::Text,*stdTextColor);
    lenText->setPalette(pal);
}

void stdLineEdit::DynSetMemoryToText(char * &t1,char * &t2)
{
    DynSetMemoryToLineEdit(t1,t2,c1,c2,displayStd,acceptLaTex,lenText);
    /*
QString text(lenText->text());
displayStd=true;
char * te=new char[text.length()+8];
strcpy(te,text.toLatin1().constData());
t2=copy_string(t2,te);//t2 is always the original
    if (acceptLaTex==true && activateLaTeXsupport==true)
    {
    complete_LaTeX_to_Grace_Translator(text);
    delete[] te;
    te=new char[text.length()+8];
    strcpy(te,text.toLatin1().constData());
    }
t1=copy_string(t1,te);//t1 is the Grace-version of the text
c1=t1;
c2=t2;
delete[] te;
*/
}

void stdLineEdit::ReplaceNumberContents(void)
{
if (OldDecimalPoint==DecimalPointToUse) return;//nothing to change!
QString te=lenText->text();
Replace_Dec_Sep_In_Single_String(te);
lenText->setText(te);
}

void stdLineEdit::RedisplayContents(void)
{
    if (c1==NULL || c2==NULL)//NULL-addresses are critical --> close a text-editor in this case
    {
        emit(NoMoreValidAdr());
    }
    else
    {
        SetLineEditToMemory(c1,c2,c1,c2,displayStd,lenText);
    }
}

void stdLineEdit::setDoubleValue(double val)
{
setDoubleValue(sformat,val);
}

void stdLineEdit::setDoubleValue(const char * form,double val)
{
    static char buf[128];
    sprintf(buf, form, val);
    SetDecimalSeparatorToUserValue(buf);
    lenText->setText(QString(buf));
}

double stdLineEdit::getDoubleValue(void)
{
    //static int strlen;
    //static char * str;
    static double d;
    //strlen=4*lenText->text().length();
    //str=new char[strlen];
    //strcpy(str,lenText->text().toLatin1().constData());
    //d=GetDoubleValueFromString(str);
    //delete[] str;
    xv_evalexpr(lenText,&d);
    return d;
}

int stdLineEdit::getIntValue(void)
{
    static int strlen;
    static char * str;
    static int d;
    strlen=4*lenText->text().length();
    str=new char[strlen];
    strcpy(str,lenText->text().toLatin1().constData());
    d=GetIntValueFromString(str);
    delete[] str;
    return d;
}

double stdLineEdit::guessDoubleValue(void)
{
    static char buf[64];
    char f,l;
    int len;
    QString help;
    strncpy(buf,lenText->text().toLatin1().constData(),63);
    buf[63]='\0';
    len=strlen(buf);
    f=l='\0';
    for (int i=0;i<len;i++)
    {
        if (buf[i]=='.' || buf[i]==',')
        {
            if (f=='\0')
            {
                f=buf[i];
            }
            else
            {
                l=buf[i];
            }
        }
    }
    if ( !( (f==l && f=='\0') || (f=='.' && l=='\0') ) )//either no decimal separator found or only a '.' --> all right --> do not(!) go into this "if"
    {
        help=QString::fromLatin1(buf);
        if (l=='\0')//--> we only found a single ',' --> we guess that this should be the decimal separator and replace it by '.'
        {
            help.replace(QString(","),QString("."));
        }
        else if (l=='.' && f==',')//we found a string with (possibly) ',' as thousand-separator and '.' as decimal separator --> remove ','
        {
            help.remove(QString(","));
        }
        else if (l==',' && f=='.')//we found a string with (possibly) '.' as thousand-separator and ',' as decimal separator --> remove '.' and replace ',' by '.'
        {
            help.remove(QString("."));
            help.replace(QString(","),QString("."));
        }
        else if (f==l)// we found multiple '.' or ',' or a constallation like 1.345,678.9 which does not make sense! --> remove ALL '.' and ','
        {
            help.remove(QString("."));
            help.remove(QString(","));
        }
        strcpy(buf,help.toLatin1().constData());
    }
    return atof(buf);
}

void stdLineEdit::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void stdLineEdit::dropEvent(QDropEvent * e)
{
e->accept();
if (!e->mimeData()->hasUrls()) return;
int len=e->mimeData()->urls().length();
QList<QUrl> list=e->mimeData()->urls();
QString n_text;
n_text.clear();
for (int i=0;i<len;i++)
{
//cout << "file=" << list.at(i).toLocalFile().toLocal8Bit().constData() << endl;
n_text+=list.at(i).toLocalFile();
if (i<len-1) n_text+=QString(";");
}
lenText->setText(n_text);
emit(changed());
}

dirList::dirList(QWidget * parent,int type):QTreeView(parent)
{
    listtype=type;

    model = new QDirModel();

    if (listtype==DIRLIST)
        model->setFilter(QDir::AllDirs);
    else
        model->setFilter(QDir::Files | QDir::NoDotAndDotDot);

    setModel(model);
    setRootIndex(model->index(QDir::currentPath()));
}

FileSelector::FileSelector(QWidget * parent):QWidget(parent)
{
    int number=4;
    QString entr[4];
    entr[0]=tr("Cwd");
    entr[1]=tr("Home");
#ifdef MAC_SYSTEM
    entr[2]=tr("/");
#endif
#ifdef LINUX_SYSTEM
    entr[2]=tr("/");
#endif
#ifdef WINDOWS_SYSTEM
    entr[2]=tr("C:\\");
    onHighestLevel=false;
#endif
    entr[3]=tr("QtGrace-dir");
    grpChDir=new QGroupBox(QString(""),this);
    selChdir=new StdSelector(grpChDir,tr("Chdir to:"),number,entr);
    cmdSetCwd=new QPushButton(tr("Set as cwd"),grpChDir);
    //cmdGetCwd=new QPushButton(tr("Get cwd"),grpChDir);
    chkShowHiddenFiles=new QCheckBox(tr("Show hidden files"),this);

    lblFilter=new QLabel(tr("Filter:"),this);
    lblDirs=new QLabel(tr("Directories:"),this);
    lblFiles=new QLabel(tr("Files:"),this);
    //ledFilter=new QLineEdit("",this);
    ledFilter=new stdLineEdit(this,tr(""));
    ledFilter->lblText->setVisible(false);
    ledFilter->layout->setMargin(0);
    ledFilter->layout->setSpacing(0);
    ledFilter->setAcceptDrops(true);
    DirList=new QListView(this);
    FileList=new QListView(this);
    modelDirs=new QDirModel();
    modelDirs->setFilter(QDir::AllDirs);
    modelFiles=new QDirModel();
    modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Drives | QDir::System);
    DirList->setModel(modelDirs);
    FileList->setModel(modelFiles);

    currentDir=QDir::currentPath();//Just for initialization

    DirList->setRootIndex(modelDirs->index(currentDir));
    FileList->setRootIndex(modelFiles->index(currentDir));
    //cmdGoUp=new QPushButton(tr("GoUp"),this);
    cmdGoUp=new QPushButton(qApp->style()->standardPixmap(QStyle::SP_FileDialogToParent),QString(""),this);
    //cmdStdDialog=new QPushButton(tr("StdDialog"),this);
    cmdStdDialog=new QPushButton(qApp->style()->standardPixmap(QStyle::SP_DirOpenIcon),QString(""),this);

    connect(cmdGoUp,SIGNAL(clicked()),SLOT(doGoUp()));
    connect(cmdStdDialog,SIGNAL(clicked()),SLOT(doStdDialog()));

    //connect(selChdir->cmbSelect,SIGNAL(currentIndexChanged(int)),SLOT(currentDirChanged(int)));
    connect(selChdir->cmbSelect,SIGNAL(highlighted(int)),SLOT(currentDirChanged(int)));
    connect(FileList,SIGNAL(clicked(const QModelIndex &)),SLOT(FileSelected(const QModelIndex &)));
    connect(FileList,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(FileDoubleClicked(const QModelIndex &)));
    connect(DirList,SIGNAL(clicked(const QModelIndex &)),SLOT(DirSelected(const QModelIndex &)));
    connect(DirList,SIGNAL(doubleClicked(const QModelIndex &)),SLOT(DirDoubleClicked(const QModelIndex &)));
    //connect(ledFilter,SIGNAL(returnPressed()),SLOT(newFilterEntered()));
    connect(ledFilter,SIGNAL(changed()),SLOT(newFilterEntered()));
    connect(chkShowHiddenFiles,SIGNAL(stateChanged(int)),SLOT(toggleHiddenFile(int)));
    connect(cmdSetCwd,SIGNAL(clicked()),SLOT(doSetCWD()));
    //connect(cmdGetCwd,SIGNAL(clicked()),SLOT(doGetCWD()));

    layout0=new QHBoxLayout;
    layout0->setMargin(2);
    layout0->addWidget(selChdir);
    layout0->addWidget(cmdSetCwd);
    //layout0->addWidget(cmdGetCwd);
    grpChDir->setLayout(layout0);

    layout=new QGridLayout();
    layout->setMargin(2);
    layout->addWidget(lblFilter,0,0,1,4);
    layout->addWidget(ledFilter,1,0,1,4);
    layout->addWidget(lblDirs,2,0);
    layout->addWidget(cmdGoUp,2,1);
    layout->addWidget(lblFiles,2,2,1,2);
    layout->addWidget(DirList,3,0,1,2);
    layout->addWidget(FileList,3,2,1,2);
    layout->addWidget(chkShowHiddenFiles,4,0,1,2);
    layout->addWidget(cmdStdDialog,4,2,1,2);
    layout->addWidget(grpChDir,5,0,1,4);
    setLayout(layout);
    doubleclickdir=false;
    doubleclickfile=false;

    separator=QDir::separator();//QString("/");

    filterExtension=QString("*.dat");
    filter=currentDir+separator+filterExtension;
    ledFilter->setText(filter);

    cmdSetCwd->setAutoDefault(false);
    cmdSetCwd->setDefault(false);
    cmdGoUp->setAutoDefault(false);
    cmdGoUp->setDefault(false);
    cmdStdDialog->setAutoDefault(false);
    cmdStdDialog->setDefault(false);
}

void FileSelector::FileSelected(const QModelIndex & index )
{
    if (doubleclickfile==true)
    {
        doubleclickfile=false;
        return;
    }
    selectedFile=index.data().toString();
//qDebug() << "FileSelected (raw)=" << selectedFile;
    QString toEmit=QDir::cleanPath(currentDir+separator+selectedFile);
//qDebug() << "FileSelected (path complete)=" << toEmit;
    toEmit=QDir::toNativeSeparators(toEmit);
//qDebug() << "FileSelected (path corrected)=" << toEmit;
    emit(newSelection(toEmit));
}

void FileSelector::FileDoubleClicked( const QModelIndex & index )
{
    doubleclickfile=true;
    selectedFile=index.data().toString();
    QString toEmit=QDir::cleanPath(currentDir+separator+selectedFile);
    toEmit=QDir::toNativeSeparators(toEmit);
    emit(newSelectionDoubleClick(toEmit));
}

void FileSelector::DirSelected(const QModelIndex & index )
{
    if (doubleclickdir==true)
    {
        doubleclickdir=false;
        return;
    }
    selectedDir=index.data().toString();
    QDir curDir(currentDir+separator+selectedDir);
    QString newFilter,newDir;
    GeneratePathWithExtension(curDir,newFilter,newDir);
    newFilter=QDir::toNativeSeparators(newFilter);
    filter=newFilter;
    ledFilter->lenText->setText(filter);
    emit(newSelection(newDir));
}

void FileSelector::DirDoubleClicked( const QModelIndex & index )
{
    doubleclickdir=true;
    selectedDir=index.data().toString();
    if(selectedDir.compare(".")==0) return;//no directory change
    currentDir=currentDir+separator+selectedDir;//set a new directory
    QString newFilter,newDir;
    GeneratePathWithExtension(currentDir,newFilter,newDir);

    QDir d1(currentDir);
    d1.makeAbsolute();
    currentDir=d1.path();
    newFilter=QDir::toNativeSeparators(newFilter);
    filter=newFilter;
    ledFilter->lenText->setText(filter);

    showFilesLikeFilter();

    /*TODO: IS THIS GOOD? See GeneratePathWithExtension(...)*/
    //newDir=newDir+separator;
    emit(newSelection(newDir));
    newFilterEntered();
}

void FileSelector::showFilesLikeFilter(void)
{
    QStringList filters;
    filters << filterExtension;
    modelDirs->setNameFilters(filters);
    modelFiles->setNameFilters(filters);
    DirList->setRootIndex(modelDirs->index(currentDir));
    FileList->setRootIndex(modelFiles->index(currentDir));
}

void FileSelector::newFilterEntered(void)
{
    ///test for "exists"???
    QString entered=ledFilter->lenText->text();
    if (entered.isEmpty())
    {
    ledFilter->lenText->setText(QDir::toNativeSeparators(QString(user_home_dir)+QDir::separator()+QString("*")));
    entered=ledFilter->lenText->text();
    }
    QDir d1(entered);
    QChar last_char=entered.at(entered.length()-1);
#ifdef WINDOWS_SYSTEM
    onHighestLevel=false;
#endif
    if ( !entered.contains(QString("*")) && !entered.contains(QString("?")) && last_char==separator.at(0))
    {
        filterExtension=QString("*");
        ledFilter->lenText->setText(QDir::toNativeSeparators(entered + filterExtension));
    }
    else
    {
        filterExtension=d1.dirName();
    }
    filter=entered;

    int pos=entered.lastIndexOf(separator);
    QString p=entered;
    p.chop(entered.length()-pos);

    QDir d2(p);
    d2.makeAbsolute();
    currentDir=d2.path();
//qDebug() << "newFilterEntered=" << filter;
    showFilesLikeFilter();
    QFileInfo fi1(filter);
emit(newSelection(fi1.absoluteDir().absolutePath()+QDir::separator()));
}

void FileSelector::setFilterFromExtern(QString & directory,QString & extension)
{
    currentDir=directory;
    filterExtension=extension;
    showFilesLikeFilter();
}

void FileSelector::setFileSelectionFromExtern(QString n_file)
{
QFileInfo fi(n_file);
QString ext=QString("*.")+fi.suffix();
if (fi.suffix().isEmpty()) ext=QString("*.agr");
QString n_dir=QDir::toNativeSeparators(fi.absolutePath());
setFilterFromExtern(n_dir,ext);

QModelIndex mi=modelFiles->index(n_file);
/// qDebug() << "Extern: file=" << n_file << " index=" << mi.row() << "" << modelFiles->rowCount();
FileList->setCurrentIndex(mi);
/*
cout << "Files: rows=" << modelFiles->rowCount() << endl;
cout << "Files: cols=" << modelFiles->columnCount() << endl;
cout << "Dirs: rows=" << DirList->model()->rowCount() << endl;
*/
}

void FileSelector::doSetCWD(void)
{
    char * dummy=new char[currentDir.length()+1];
    strcpy(dummy,currentDir.toLatin1());
    set_workingdir(dummy);
    delete[] dummy;
}

/*void FileSelector::doGetCWD(void)
{
    currentDir=QString(get_workingdir());
    currentDir.chop(1);
    filter=currentDir+separator+filterExtension;
    ledFilter->setText(QDir::toNativeSeparators(filter));
    showFilesLikeFilter();
}*/

void FileSelector::toggleHiddenFile(int i)
{
    if (i==Qt::Checked)
    {
        modelDirs->setFilter(QDir::AllDirs | QDir::Hidden);
        modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
    }
    else
    {
        modelDirs->setFilter(QDir::AllDirs);
        modelFiles->setFilter(QDir::Files | QDir::NoDotAndDotDot);
    }
    showFilesLikeFilter();
}

void FileSelector::currentDirChanged(int i)
{
    switch (i)
    {
    case 0://CWD
        currentDir=QString(get_workingdir());
        currentDir.chop(1);
        break;
    case 1://HOME
        currentDir=QString(get_userhome());
        currentDir.chop(1);
#ifdef WINDOWS_SYSTEM
        onHighestLevel=true;
#endif
        break;
    case 2://'/'
#ifdef WINDOWS_SYSTEM
        currentDir=QString("C:\\");
#else
        currentDir=QString("");
#endif
        break;
    case 3:
        currentDir=QString(qt_grace_share_dir);
        break;
    }
    filter=currentDir+separator+filterExtension;
    ledFilter->setText(QDir::toNativeSeparators(filter));
    showFilesLikeFilter();
}

void FileSelector::GeneratePathWithExtension(QDir dir,QString & PathAndExtension,QString & DirectoryOnly)
{
    QString str=dir.cleanPath(dir.absolutePath()).toLatin1();
    QString current;
    if (str.startsWith("/.."))
    {
        while (str.startsWith("/.."))
        {
            current=str.right(str.length()-3);
            str=current;
        }
    }
    else
    {
        current=dir.absolutePath();
    }
#ifdef WINDOWS_SYSTEM
    while (current.count(QString(":"))>1)
    {
        str=current;
        current=str.right(str.length()-3);
    }
    if (current.contains(QString(":")))
    {
        current=current.mid(current.indexOf(QString(":"))-1);
    }
    onHighestLevel=false;
#endif
    PathAndExtension=current+separator+filterExtension;
    PathAndExtension=QDir::cleanPath(PathAndExtension);
    PathAndExtension=QDir::toNativeSeparators(PathAndExtension);
    DirectoryOnly=current+separator;
    DirectoryOnly=QDir::cleanPath(DirectoryOnly);
    DirectoryOnly=QDir::toNativeSeparators(DirectoryOnly)+separator;
}

void FileSelector::doGoUp(void)
{
    QDir cdir(currentDir);
    QDir tdir(cdir);
    /*QString str2=currentDir;
char strbuf[512];
strcpy(strbuf,str2.toLatin1());
if (strncmp(strbuf,"/..",3)==0)
{
char buf2[512];
strcpy(buf2,strbuf+3);
cout << "oh oh" << endl;
strcpy(strbuf,cdir.cleanPath(tdir.absolutePath()).toLatin1());
cout << "buf=" << strbuf << endl;
tdir=QDir(buf2);
}
cout << "currentdir=#" << strbuf << "#" << endl;
QString toEmit;
str2.chop(1+cdir.dirName().length());
//currentDir=str2;
strcpy(strbuf,str2.toLatin1());
cout << "updir1=#" << strbuf << "#" << endl;
*/
    bool up_possible=tdir.cdUp();
    /*strcpy(strbuf,tdir.path().toLatin1());
cout << "updir2=#" << strbuf << "# possible=" << up_possible << endl;*/
#ifdef WINDOWS_SYSTEM
    if (up_possible==false || onHighestLevel==true)
#else
    if (up_possible==false)
#endif
    {
#ifdef WINDOWS_SYSTEM
        currentDirChanged(1);
#endif
        return;
    }

    currentDir=tdir.absolutePath();
    currentDir=QDir::cleanPath(currentDir);
    QString newFilter,newPath;
    GeneratePathWithExtension(tdir,newFilter,newPath);
    filter=newFilter;
    ledFilter->setText(newFilter);
    emit(newSelection(newPath));
    /*currentDir=tdir.absolutePath();
filter=currentDir+separator+filterExtension;
toEmit=QDir::cleanPath(filter);
toEmit=QDir::toNativeSeparators(toEmit);
ledFilter->setText(toEmit);
toEmit=QDir::cleanPath(currentDir+separator);
toEmit=QDir::toNativeSeparators(toEmit);
emit(newSelection(toEmit));*/
    showFilesLikeFilter();
}

void FileSelector::doStdDialog(void)
{
QString s;
    if (forRead==true)
        s=QFileDialog::getOpenFileName(this,currentDir);
    else
        s=QFileDialog::getSaveFileName(this,currentDir);
    if (!s.isNull())
    {
        QFileInfo fi1(QDir::toNativeSeparators(s));
        QString suffix=fi1.completeSuffix();
        QString path_only=fi1.path();
        ledFilter->lenText->setText(fi1.absolutePath()+QDir::separator()+QString("*.")+fi1.suffix());
        setFilterFromExtern(path_only,suffix);
        setFileSelectionFromExtern(s);
        emit(newSelection(s));
        qApp->processEvents();
        emit(newSelectionManually());
        //qApp->processEvents();
        //emit(newSelection(s));
        return;
/// qDebug() << "StdDialog: s=" << s;
        /*lazy method of finding filename and directory*/
        ledFilter->lenText->setText(QDir::toNativeSeparators(s));
        newFilterEntered();
        //set and announce new selection and filters
        selectedFile=filter;
        //QFileInfo fi1(selectedFile);
        filterExtension=QString("*.")+fi1.completeSuffix();
        filter=currentDir+separator+filterExtension;
        emit(newSelection(selectedFile));
        //show selection again
        ///modify to do this only for existing files!!!
        ledFilter->lenText->setText(QDir::toNativeSeparators(filter));
        newFilterEntered();
/// qDebug() << "StdDialog: selectedFile=" << selectedFile;
        QModelIndex index=modelFiles->index(selectedFile);
        /*if (index.row()<0)
        {
        QModelIndex sav_index=index;
        index=modelFiles->index(fi1.fileName());
qDebug() << "StdDialog: Filename only=" << fi1.fileName();
        if (index.row()<0) index=sav_index;
        }*/
/// qDebug() << "index=" << index.row();
        FileList->setCurrentIndex(index);
        /*QString suffix=fi1.completeSuffix();
        QString path_only=fi1.path();
        setFilterFromExtern(path_only,suffix);
        setFileSelectionFromExtern(s);*/
    }
}

stdStartStop::stdStartStop(QWidget * parent):QWidget(parent)
{
    lblStart=new QLabel(tr("Start load at:"),this);
    lblStop=new QLabel(tr("Stop load at:"),this);
    lblLength=new QLabel(tr("# of points:"),this);
    ledStart=new QLineEdit(QString(""),this);
    ///ledStart->setValidator(new QDoubleValidator(this));
    ledStop=new QLineEdit(QString(""),this);
    //ledStop->setValidator(new QDoubleValidator(this));
    ledLength=new QLineEdit(QString(""),this);
    QIntValidator * intval=new QIntValidator(this);
    intval->setBottom(1);
    ledLength->setValidator(intval);
    //ledLength->setValidator(new QRegExpValidator(rx, this));
    error=false;//state indicating conversion errors
    layout=new QHBoxLayout();
    layout->setMargin(2);
    layout->addWidget(lblStart);
    layout->addWidget(ledStart);
    layout->addWidget(lblStop);
    layout->addWidget(ledStop);
    layout->addWidget(lblLength);
    layout->addWidget(ledLength);
    setLayout(layout);
}

void stdStartStop::Redisplay(void)
{
    QString te;
    if (OldDecimalPoint==DecimalPointToUse) return;
    te=ledStart->text();
    Replace_Dec_Sep_In_Single_String(te);
    ledStart->setText(te);
    te=ledStop->text();
    Replace_Dec_Sep_In_Single_String(te);
    ledStop->setText(te);
}

double stdStartStop::start(void)
{
    double val;
    error=xv_evalexpr(ledStart, &val)==RETURN_SUCCESS?false:true;
    /*int len=ledStart->text().length();
if (len<=0)
{
error=true;
return 0.0;
}
char dummy[ledStart->text().length()];
strcpy(dummy,ledStart->text().toLatin1());
return atof(dummy);*/
    return val;
}

double stdStartStop::stop(void)
{
    double val;
    error=xv_evalexpr(ledStop, &val)==RETURN_SUCCESS?false:true;
    /*int len=ledStop->text().length();
if (len<=0)
{
error=true;
return 0.0;
}
char dummy[ledStop->text().length()];
strcpy(dummy,ledStop->text().toLatin1());
return atof(dummy);*/
    return val;
}

int stdStartStop::length(void)
{
    int val;
    error=xv_evalexpri(ledLength,&val)==RETURN_SUCCESS?false:true;
    /*int len=ledLength->text().length();
if (len<=0)
{
error=true;
return 0.0;
}
char dummy[ledLength->text().length()];
strcpy(dummy,ledLength->text().toLatin1());
return atoi(dummy);*/
    return val;
}

stdSetTypeSelector::stdSetTypeSelector(QWidget * parent):StdSelector(parent)
{
    lblText=new QLabel("Type:");
    cmbSelect=new QComboBox(this);
    number_of_entries=column_count=2;
    entries=new QString[column_count];
    entryValues=new int[column_count];
    entries[1]=QString("");
    entries[0]=QString("");
    entryValues[0]=0;
    entryValues[1]=1;
    cmbSelect->addItem(entries[0]);
    cmbSelect->addItem(entries[1]);
    layout=new QHBoxLayout;
    layout->setMargin(2);
    layout->addWidget(lblText);
    layout->addWidget(cmbSelect);
    setLayout(layout);
    showAll=false;
    setColumnCount(column_count);
}

void stdSetTypeSelector::setColumnCount(int count)
{
    if (showAll==false)
        column_count=count;
    char dummy[512];
    ///SET APPROPRIATE SET_TYPE_CHOICES ACCORDING TO NUMBER OF COLUMNS --> SHOW ONLY AVAILABLE CHOICES, I DON'T KNOW HOW TO DISABLE CHOICES
    int number_of_Type_entries=0;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
        if (settype_cols(i) == count || showAll==true) number_of_Type_entries++;
    int * Type_entries=new int[number_of_Type_entries];
    QString * entr=new QString[number_of_Type_entries];
    number_of_Type_entries=0;
    for (int i=0;i<NUMBER_OF_SETTYPES;i++)
    {
        if (settype_cols(i) == count || showAll==true)
        {
            strToUpper(dummy,set_types(i));
            entr[number_of_Type_entries]=QString(dummy);
            Type_entries[number_of_Type_entries]=i;
            number_of_Type_entries++;
        }
    }
    setNewEntries(number_of_Type_entries,entr,Type_entries);
}

void stdSetTypeSelector::setShowAll(bool sa)
{
    showAll=sa;
    setColumnCount(column_count);
}

int stdSetTypeSelector::getColumnCount(void)
{
    return settype_cols(currentValue());
}

SetTableView::SetTableView(int gno,int sno,int * colformat,int * colprecision,QWidget * parent):QTableView(parent)
{
    model=new SetTableModel(gno,sno,colformat,colprecision,parent);
    setModel(model);
}

bool SetTableView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
    //int row=index.row();
    //int col=index.column();
    //cout << "edit:" << row << "|" << col << endl;
    bool ret=QTableView::edit(index,trigger,event);
    return ret;
}

void SetTableView::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    (void)roles;
    //cout << "data changed" << endl;
    QTableView::dataChanged(topLeft, bottomRight);
}

SetTableModel::SetTableModel(int gno,int sno,int * colformat,int * colprecision,QObject * parent):QAbstractTableModel(parent)
{
    setnr=sno;
    graphnr=gno;
    col_format=colformat;
    col_precision=colprecision;
}

bool SetTableModel::init(int gno,int sno)
{
    if (!is_valid_setno(gno,sno)) return false;
    setnr=sno;
    graphnr=gno;
    return true;
}

int SetTableModel::rowCount(const QModelIndex & parent) const
{
    (void)parent;
    if (!is_valid_setno(graphnr,setnr))
        return 0;
    else
        return getsetlength(graphnr,setnr)+10;//10 empty lines
}

int SetTableModel::columnCount(const QModelIndex & parent) const
{
    (void)parent;
    if (!is_valid_setno(graphnr,setnr))
        return 0;
    else
        return dataset_cols(graphnr,setnr);
}

QVariant SetTableModel::data(const QModelIndex & index,int role) const
{
    static char dummy[128];
    static int row,col;
    static QVariant dat;
    dat=QVariant(QVariant::Invalid);
    if (!is_valid_setno(graphnr,setnr)) return dat;
    row=index.row();
    col=index.column();
    //cout << "Data" << endl;
    if (role==Qt::DisplayRole || role==Qt::EditRole)
    {
        //cout << "Data=VALID" << endl;
        if (row>=getsetlength(graphnr,setnr)) return QVariant(QString(""));
        construct_string_from_data(g[graphnr].p[setnr].data.ex[col][row],dummy,col_precision[col],col_format[col]);
        return QVariant(QString(dummy));
    }
    else
    {
        //cout << "Data=invalid" << endl;
        return QVariant::Invalid;
    }
}

void construct_string_from_data(double value,char * target,int col_precision,int col_format)
{
    static char setPrec[8];
    static char out_format[16];
    static char dummy[128];
    //cout << "construct: value=" << value << endl;
    sprintf(setPrec,"%%.%d",col_precision);
    switch (col_format)
    {
    case 0://decimal
        sprintf(out_format,"%sf",setPrec);
        break;
    case 1://general
        sprintf(out_format,"%sg",setPrec);
        break;
    case 2://exponential
        sprintf(out_format,"%se",setPrec);
        break;
    case 3://Date/time
        sprintf(out_format,"%%t");///NOT CORRECT
        break;
    }
    sprintf(dummy,out_format,value);
    if (col_format!=3)//no changes if it is in date/time-format, otherwise-->correct the decimal separator
    {
        SetDecimalSeparatorToUserValue(dummy);
    }
    strcpy(target,dummy);
}

bool SetTableModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
    static int row,col;
    static double val;
    static bool ok,really_new_value,set_is_new;
    static QVariant n_val;
    char * oldval,*newval;
    ok=true;
    //cout << "SetData" << endl;
    if (!is_valid_setno(graphnr,setnr)) return false;//invalid set
    really_new_value=false;
    val=value.toDouble(&ok);
    if (role==Qt::DisplayRole || !is_valid_setno(graphnr,setnr) || ok==false) return false;
    row=index.row();
    col=index.column();
    oldval=new char[256];
    newval=new char[256];
    strcpy(newval,value.toString().toLatin1().constData());
    //cout << "newValue=" << newval << endl;
    if (row<getsetlength(graphnr,setnr))//data already exists --> do we need to override it?
    {
        construct_string_from_data(g[graphnr].p[setnr].data.ex[col][row],oldval,col_precision[col],col_format[col]);
        if (!strcmp(newval,oldval))
            really_new_value=false;
        else
            really_new_value=true;
    }
    else
        really_new_value=true;
    ReplaceDecimalSeparator(newval);
    //cout << "a) value=" << value.toString().toLatin1().constData() << endl;
    n_val=QVariant(QString(newval));
    //cout << "b) value=" << n_val.toString().toLatin1().constData() << endl;
    delete[] oldval;
    delete[] newval;
    if (really_new_value==false) return true;//nothing has actually to be changed
    val=n_val.toDouble(&ok);
    if (ok==false)
    {
        cerr << "Invalid double-conversion from #" << n_val.toString().toLatin1().constData() << "#" << endl;
        return false;//invalid conversion
    }
    if (row<getsetlength(graphnr,setnr))//change an existing data point
    {
        int len=1,ncols;
        int * nrs=new int[2];
        nrs[0]=row;
        Datapoint * po=new Datapoint[2];
        //int ret=get_datapoint(graphnr,setnr,row,&ncols,po);
        (void)get_datapoint(graphnr,setnr,row,&ncols,po);
        po->ex[col]=val;
        DataPointEdited(graphnr,setnr,nrs,len,po,2);
        g[graphnr].p[setnr].data.ex[col][row]=val;
        delete[] nrs;
        delete[] po;
    }
    else//adding rows (filled with zeros) and set the last one to the new value
    {
        if (getsetlength(graphnr,setnr)<=0)
            set_is_new=true;
        else
            set_is_new=false;
        Datapoint dpoint;
        zero_datapoint(&dpoint);
        int len=row-getsetlength(graphnr,setnr)+1;//,ncols;
        int counter=0;
        int * nrs=new int[len+1];
        Datapoint * po=new Datapoint[len+1];
        for (int i=0;i<len;i++)
        {
            zero_datapoint(po+i);
        }
        po[len-1].ex[col]=val;
        for (int i=getsetlength(graphnr,setnr);i<=row;i++)
        {
            nrs[counter++]=i;
            add_point_at(graphnr,setnr, i, &dpoint);
        }
        g[graphnr].p[setnr].data.ex[col][row]=val;
        if (set_is_new==true)
        {
            int klen=1;
            int * gnos=new int[2];
            int * snos=new int[2];
            gnos[0]=graphnr;
            snos[0]=setnr;
            SetsCreated(klen,gnos,snos,UNDO_COMPLETE);
            delete[] gnos;
            delete[] snos;
        }
        else
        {
            DataPointEdited(graphnr,setnr,nrs,len,po,0);
        }
    }
    ok=true;
    emit(item_entry_changed(row,col,really_new_value));
    return ok;
}

QVariant SetTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    static QVariant dat;
    dat=QVariant(QVariant::Invalid);
    if (role==Qt::DisplayRole)
    {
        if (orientation==Qt::Vertical)//row
        {
            dat=QVariant(section);//only the row number
        }
        else if (orientation==Qt::Horizontal)//column
        {
            if (section==0)
                dat=QVariant("X");
            else if (section==1)
                dat=QVariant("Y");
            else
            {
                char dummy[8];
                sprintf(dummy,"Y%d",section-1);
                dat=QVariant(dummy);
            }
        }
    }
    /*else
{
    cout << "role=" << (int)role << endl;
}*/
    return dat;
}

bool SetTableModel::insertRows(int row, int count, const QModelIndex & parent)
{
    beginInsertRows(parent,row,row+count-1);
    endInsertRows();
    return true;
}

bool SetTableModel::removeRows(int row, int count, const QModelIndex & parent)
{
    beginRemoveRows(parent,row,row+count-1);
    endRemoveRows();
    return true;
}

bool SetTableModel::insertColumns(int column, int count, const QModelIndex & parent)
{
    (void)column;
    (void)count;
    (void)parent;
    return false;
}

bool SetTableModel::removeColumns(int column, int count, const QModelIndex & parent)
{
    (void)column;
    (void)count;
    (void)parent;
    return false;
}

Qt::ItemFlags SetTableModel::flags( const QModelIndex & index ) const
{
    if (index.isValid())
        return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    else
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void SetTableModel::major_changes(void)
{
    emit(layoutChanged());
}

/* Wrappers */
int GetSpinChoice(stdIntSelector * s)
{
    return s->value();
}

double GetSpinChoice(LineWidthSelector * s)
{
    return s->value();
}

double GetSpinChoice(QDoubleSpinBox * s)	
{
    return s->value();
}

bool GetToggleButtonState(QCheckBox * c)
{
    return c->isChecked();
}

int GetChoice(StdSelector * s)
{
    return s->currentValue();
}

int GetChoice(PositionSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(ColorSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(LineStyleSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(FillPatternSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(JustificationSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(FontSelector * s)
{
    return s->currentIndex();
}

int GetOptionChoice(QComboBox * c)
{
    return c->currentIndex();
}

int GetOptionChoice(StdSelector * sel)
{
    return sel->currentValue();
}

double GetCharSizeChoice(stdSlider * sel)
{
    return sel->value();
}

double GetAngleChoice(stdSlider * sel)
{
    return sel->value();
}

char * GetTextString(stdLineEdit * led)
{
    static char dummy[MAX_STRING_LENGTH];
    //strcpy(dummy,led->lenText->text().toLatin1());
    //strcpy(dummy,led->text().toLatin1());
    strcpy(dummy,led->text().toUtf8().constData());
    return dummy;
}

void SetToggleButtonState(QCheckBox * cb, int v)
{
    cb->setChecked(bool(v));
}

void SetToggleButtonState(QAction * c, int v)
{
    c->setChecked(bool(v));
}

bool GetToggleButtonState(QAction * c)
{
    return c->isChecked();
}

void SetChoice(StdSelector * sel,int v)
{
    sel->setCurrentValue(v);
}

void SetOptionChoice(ColorSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(LineStyleSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(FillPatternSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(FontSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(JustificationSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetOptionChoice(StdSelector * sel,int v)
{
    sel->setCurrentValue(v);
}

void SetSpinChoice(stdIntSelector * sel, double v)
{
    sel->setValue((int)v);
}

void SetSpinChoice(LineWidthSelector * sel, double v)
{
    sel->setValue(v);
}

void SetChoice(PositionSelector * sel,int v)
{
    sel->setCurrentIndex(v);
}

void SetScaleValue(stdSlider * sel, int v)
{
    sel->setValue(v);
}

void SetCharSizeChoice(stdSlider * sel,double v)
{
    sel->setValue(int(rint_2(v)));
}

void SetAngleChoice(stdSlider * sel,double v)
{
    sel->setValue(int(rint_2(v)));
}

void SetTextString(stdLineEdit * led,const char * st)
{
    led->setText(st);
}

void SetLabel(QLabel * lab,const char * st)
{
    lab->setText(QString(st));
}

void xv_setstr(stdLineEdit * led,const char * text)
{
    led->setText(QString(text));
}

void xv_setstr(QLineEdit * led,const char * text)
{
    led->setText(QString(text));
}

void SetSensitive(stdLineEdit * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(QLineEdit * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(StdSelector * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(QPushButton * led, bool e)
{
    led->setEnabled(e);
}

void SetSensitive(QCheckBox * led, bool e)
{
    led->setEnabled(e);
}

int GetScaleValue(stdSlider * sel)
{
    return sel->value();
}

char * xv_getstr(stdLineEdit * led)
{
    /// WARNING POSSIBLY UNSAVE PROCEDURE!!!
    static char value[1024];
    strncpy(value,led->lenText->text().toLatin1(),1023);
    value[1023]='\0';
    return value;
}

char * xv_getstr(QLineEdit * led)
{
    /// WARNING POSSIBLY UNSAVE PROCEDURE!!!
    static char value[1024];
    strncpy(value,led->text().toLatin1(),1023);
    value[1023]='\0';
    return value;
}

int xv_evalexpr(stdLineEdit * w, double * answer )
{
    return xv_evalexpr(w->lenText,answer);
}

int xv_evalexpri(stdLineEdit * w,int * answer)
{
    return xv_evalexpri(w->lenText,answer);
}

/*
 * xv_evalexpr - take a text field and pass it to the parser if it needs to be
 * evaluated, else use atof().
 * place the double result in answer
 * if an error, return False, else True
 */
int xv_evalexpr(QLineEdit * w,double * answer)
{
static char *buf = NULL;
int ret=RETURN_FAILURE;
QString st=w->text();
buf=new char[st.length()+1];
strcpy(buf,st.toLatin1());//we assume, that all fields that have numbers in it for use as numbers can safely be interpreted in the iso-latin1-codec
ret=std_evalexpr(buf,answer);
delete[] buf;
return ret;
}

int std_evalexpr(char * buf,double * answer)
{
int i, len, ier = 0;
double result;

    ReplaceDecimalSeparator(buf);

    if (!(len = strlen( buf ) )) { /* check for zero length */
        *answer = 0;
        return RETURN_FAILURE;
    }
    /* first character may be a sign */
    if (!fpdigit[(int) buf[0]] && buf[0] != '-' && buf[0] != '+') {
        i = len +1;
    } else {
        i = 1;
    }

    for (; i<len; i++) {
        if (!fpdigit[(int) buf[i]]) {
            break;
        }
    }

    if (i == len) {         /* only floating point digits */
        *answer = atof( buf );
        return RETURN_SUCCESS;
    } else {                /* must evaluate an expression */
        ier = s_scanner(buf, &result);
        if( !ier ) {
            *answer = result;
            return RETURN_SUCCESS;
        } else {
            *answer = 0;
            return RETURN_FAILURE;
        }
    }
}

/*
 * xv_evalexpri - take a text field and pass it to the parser if it needs to
 * evaluated, else use atoi().
 * place the integer result in answer
 * if an error, return False, else True
 */
int xv_evalexpri(QLineEdit * w,int * answer)
{
    static char *buf = NULL;
    int i, len, ier = 0;
    double result;
    QString st=w->text();
    buf=new char[st.length()+1];
    strcpy(buf,st.toLatin1());
    ReplaceDecimalSeparator(buf);

    if (!(len = strlen( buf ) )) { /* check for zero length */
        *answer = 0;
        return RETURN_FAILURE;
    }
    /* first character may be a sign */
    if (!fpdigit[(int) buf[0]] && buf[0] != '-' && buf[0] != '+') {
        i = len +1;
    } else {
        i = 1;
    }

    for (; i<len; i++) {
        if (!fpdigit[(int) buf[i]]) {
            break;
        }
    }

    if (i == len) {             /* only floating point digits */
        *answer = atoi(buf);
        return RETURN_SUCCESS;
    } else {                    /* must evaluate an expression */
        ier = s_scanner(buf, &result);
        if( !ier ) {
            *answer = (int)result;
            return RETURN_SUCCESS;
        } else {
            *answer = 0;
            return RETURN_FAILURE;
        }
    }
}

void SelectListChoice(uniList * list,int nr)
{
    int count=1;
    int selection[2]={nr,1};
    list->set_new_selection(count,selection);
}

void SelectListChoices(uniList * list,int count,int * nrs)
{
    list->set_new_selection(count,nrs);
}

int GetListChoices(uniList * list, int ** sel)
{
    int nr;
    list->get_selection(&nr,sel);
    return nr;
}

int GetSelectedSets(uniList * list, int ** sel)
{
    int nr;
    list->get_selection(&nr,sel);
    return nr;
}

int GetSelectedSet(uniList * list)
{
    int nr,*sel=new int[3];
    list->get_selection(&nr,&sel);
    if (nr>0)
        nr=sel[0];
    else
        nr=SET_SELECT_ERROR;
    delete[] sel;
    return nr;
}

int GetSingleListChoice(uniList * list, int * sel)
{
    int nr;
    int * selection=new int[5];
    list->get_selection(&nr,&selection);
    if (nr<=0)
    {
        delete[] selection;
        return RETURN_FAILURE;
    }
    else
    {
        *sel=selection[0];
        delete[] selection;
        return RETURN_SUCCESS;
    }
}

GraphPopup::GraphPopup(uniList *parent):QMenu(parent)
{
    par=parent;
    CreateActions();
    setTearOffEnabled(FALSE);

    //Create Menu itself
    addAction(actLabel);
    addSeparator();
    addAction(actFocusTo);
    addAction(actHide);
    addAction(actShow);
    addAction(actDuplicate);
    addAction(actKill);
    addSeparator();
    addAction(actCopy12);
    addAction(actCopy21);
    addAction(actMove12);
    addAction(actMove21);
    addAction(actSwap);
    addSeparator();
    addAction(actCreateNew);

    number_of_selected_graphs=0;
    selected_graphs=new int[2];
}

GraphPopup::~GraphPopup()
{
    delete[] selected_graphs;
}

void GraphPopup::CreateActions(void)
{
    actLabel=new QAction(tr("G0"),this);
    connect(actLabel,SIGNAL(triggered()), this, SLOT(doNothing()));
    actFocusTo=new QAction(tr("&Focus to"),this);
    connect(actFocusTo,SIGNAL(triggered()), this, SLOT(doFocusTo()));
    actShow=new QAction(tr("&Show"),this);
    connect(actShow,SIGNAL(triggered()), this, SLOT(doShow()));
    actHide=new QAction(tr("&Hide"),this);
    connect(actHide,SIGNAL(triggered()), this, SLOT(doHide()));
    actDuplicate=new QAction(tr("&Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()), this, SLOT(doDuplicate()));
    actKill=new QAction(tr("&Kill"),this);
    connect(actKill,SIGNAL(triggered()), this, SLOT(doKill()));
    actCopy12=new QAction(tr("Copy G0 to G1"),this);
    connect(actCopy12,SIGNAL(triggered()), this, SLOT(doCopy12()));
    actCopy21=new QAction(tr("Copy G1 to G0"),this);
    connect(actCopy21,SIGNAL(triggered()), this, SLOT(doCopy21()));
    actMove12=new QAction(tr("Move G0 to G1"),this);
    connect(actMove12,SIGNAL(triggered()), this, SLOT(doMove12()));
    actMove21=new QAction(tr("Move G1 to G0"),this);
    connect(actMove21,SIGNAL(triggered()), this, SLOT(doMove21()));
    actSwap=new QAction(tr("S&wap"),this);
    connect(actSwap,SIGNAL(triggered()), this, SLOT(doSwap()));
    actCreateNew=new QAction(tr("&Create new"),this);
    connect(actCreateNew,SIGNAL(triggered()), this, SLOT(doCreateNew()));
}

void GraphPopup::doNothing(void)
{
    par->activateHighlighting();
}

void GraphPopup::doFocusTo(void)
{
    ListOfChanges.clear();
    sprintf(dummy,"with g%d",selected_no);
    ListOfChanges << QString(dummy);
    switch_current_graph(selected_no);
    update();
}

void GraphPopup::doShow(void)
{
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
    {
        if (is_graph_hidden(selected_graphs[i])==TRUE)//real change
        {
            sprintf(dummy,"g%d hidden false",selected_graphs[i]);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden true",selected_graphs[i]);
            ListOfOldStates << QString(dummy);
        }
    }
    ShowHideGraphs(number_of_selected_graphs,selected_graphs);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
        set_graph_hidden(selected_graphs[i], FALSE);
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doHide(void)
{
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
    {
        if (is_graph_hidden(selected_graphs[i])==FALSE)//real change
        {
            sprintf(dummy,"g%d hidden true",selected_graphs[i]);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden false",selected_graphs[i]);
            ListOfOldStates << QString(dummy);
        }
    }
    ShowHideGraphs(number_of_selected_graphs,selected_graphs);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    for (int i = 0; i < number_of_selected_graphs; i++)
        set_graph_hidden(selected_graphs[i], TRUE);
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doDuplicate(void)
{
    int * gnos=new int[number_of_selected_graphs+2];
    int index=0;
    for (int i = 0; i < number_of_selected_graphs; i++)
    {
        new_set_no=-1;
        duplicate_graph(selected_graphs[i]);
        if (new_set_no!=-1)
            gnos[index++]=new_set_no;
    }
    GraphsCreated(index,gnos,UNDO_COMPLETE);
    delete[] gnos;
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doKill(void)
{
    if (yesno(QObject::tr("Kill selected graph(s)?").toLocal8Bit().data(), NULL, NULL, NULL))
    {
        ListOfChanges.clear();
        int * graphs=new int[number_of_selected_graphs];
        for (int i = number_of_selected_graphs - 1; i >= 0; i--)
        {
            sprintf(dummy,"%s G%d",QObject::tr("kill").toLocal8Bit().constData(),selected_graphs[i]);
            ListOfChanges << QString(dummy);
            graphs[(number_of_selected_graphs - 1)-i]=selected_graphs[i];
        }
        GraphsDeleted(number_of_selected_graphs,graphs,UNDO_COMPLETE);
        delete[] graphs;
        for (int i = number_of_selected_graphs - 1; i >= 0; i--)
        {
            kill_graph(selected_graphs[i]);
        }
        ListOfChanges.clear();
    }
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doSwap(void)
{
    UndoSwapGraphs(selected_graphs[0], selected_graphs[1]);
    swap_graph(selected_graphs[0], selected_graphs[1]);
    update_graph_selectors();
}

void GraphPopup::doCreateNew(void)
{
    int * gnos=new int[2];
    gnos[1]=gnos[0]=number_of_graphs();
    set_graph_active(number_of_graphs());
    GraphsCreated(1,gnos,UNDO_COMPLETE);
    delete[] gnos;
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::update(void)
{
    par->update_number_of_entries_preserve_selection();
    mainWin->mainArea->completeRedraw();
}

void GraphPopup::doMove12(void)
{
    char buf[32];
    if (number_of_selected_graphs==2)
    {
        sprintf(buf, "Replace G%d?", selected_graphs[1]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            move_graph(selected_graphs[0], selected_graphs[1]);
            selected_graphs[0]=selected_graphs[1];
            GraphsModified(1,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::doMove21(void)
{
    char buf[32];
    if (number_of_selected_graphs==2)
    {
        sprintf(buf, "Replace G%d?", selected_graphs[0]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            move_graph(selected_graphs[1], selected_graphs[0]);
            GraphsModified(1,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::doCopy12(void)
{
    char buf[32];
    if (number_of_selected_graphs == 2)
    {
        sprintf(buf, "Overwrite G%d?", selected_graphs[1]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            copy_graph(selected_graphs[0], selected_graphs[1]);
            GraphsModified(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::doCopy21(void)
{
    char buf[32];
    if (number_of_selected_graphs == 2)
    {
        sprintf(buf, "Overwrite G%d?", selected_graphs[0]);
        if (yesno(buf, NULL, NULL, NULL))
        {
            SaveGraphStatesPrevious(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
            copy_graph(selected_graphs[1], selected_graphs[0]);
            GraphsModified(number_of_selected_graphs,selected_graphs,UNDO_COMPLETE);
        }
        mainWin->mainArea->completeRedraw();
    }
}

void GraphPopup::update_menu_content(void)
{
    char dummy[64],dummy2[64];
    sprintf(dummy,"G");
    for (int i=0;i<number_of_selected_graphs;i++)
    {
        sprintf(dummy2,"%d",selected_graphs[i]);
        strcat(dummy,dummy2);
        if (i>2 && i!=number_of_selected_graphs-1)
        {
            strcat(dummy,",...");
            break;
        }
        else
        {
            if (i!=number_of_selected_graphs-1)
                strcat(dummy,", G");
        }
    }
    actLabel->setText(QString(dummy));
    if (number_of_selected_graphs==1)
    {
        actCopy12->setVisible(false);
        actCopy21->setVisible(false);
        actMove12->setVisible(false);
        actMove21->setVisible(false);
        actSwap->setEnabled(false);
        actFocusTo->setEnabled(true);
    }
    else
    {
        if (number_of_selected_graphs==2)
        {
            sprintf(dummy,"Copy G%d to G%d",selected_graphs[0],selected_graphs[1]);
            actCopy12->setText(QString(dummy));
            actCopy12->setVisible(true);
            sprintf(dummy,"Copy G%d to G%d",selected_graphs[1],selected_graphs[0]);
            actCopy21->setText(QString(dummy));
            actCopy21->setVisible(true);
            sprintf(dummy,"Move G%d to G%d",selected_graphs[0],selected_graphs[1]);
            actMove12->setText(QString(dummy));
            actMove12->setVisible(true);
            sprintf(dummy,"Move G%d to G%d",selected_graphs[1],selected_graphs[0]);
            actMove21->setText(QString(dummy));
            actMove21->setVisible(true);
            actSwap->setEnabled(true);
        }
        else
        {
            actCopy12->setVisible(false);
            actCopy21->setVisible(false);
            actMove12->setVisible(false);
            actMove21->setVisible(false);
            actSwap->setEnabled(false);
        }
        actFocusTo->setEnabled(false);
    }
}

SetPopup::SetPopup(uniList *parent):QMenu(parent)
{
    par=parent;
    number_of_selected_sets=0;
    selected_sets=new int[2];

    CreateActions();
    setTearOffEnabled(FALSE);

    //Create Menu itself
    addAction(actLabel);
    //addAction(actSyncListContents);
    addSeparator();
    addAction(actHide);
    addAction(actShow);
    addAction(actBringToFront);
    addAction(actSendToBack);
    addSeparator();
    addAction(actDuplicate);
    addAction(actKill);
    addAction(actKillData);
    addSeparator();
    addAction(actCopyClipBoard);
    addAction(actPasteClipBoard);
    addSeparator();
    addAction(actCopy12);
    addAction(actCopy21);
    addAction(actMove12);
    addAction(actMove21);
    addAction(actSwap);
    addSeparator();
    mnuEdit=new QMenu(tr("&Edit"),this);
    mnuEdit->setTearOffEnabled(FALSE);
    mnuEdit->addAction(actEditInSpreadsheet);
    mnuEdit->addAction(actEditInTextEditor);
    mnuCreateNew=new QMenu(tr("Create new"),this);
    mnuCreateNew->setTearOffEnabled(FALSE);
    mnuCreateNew->addAction(actCreateByFormula);
    mnuCreateNew->addAction(actCreateInSpreadsheet);
    mnuCreateNew->addAction(actCreateInTextEditor);
    mnuCreateNew->addAction(actCreateFromBlockData);
    addMenu(mnuEdit);
    addMenu(mnuCreateNew);
    addSeparator();
    addAction(actPackAllSets);
    addSeparator();
    addAction(actStoreAppearance);
    addAction(actApplyStoredAppearance);
    addSeparator();

    mnuDisplay=new QMenu(tr("Display options"),this);
    mnuDisplay->setTearOffEnabled(FALSE);

    mnuSelectorOperations=new QMenu(tr("Select&or operations"),this);
    mnuSelectorOperations->setTearOffEnabled(FALSE);

    actViewIcons->setCheckable(true);
    actViewIcons->setChecked(FALSE);
    actViewSetComments->setCheckable(true);
    actViewSetComments->setChecked(FALSE);
    actViewSetLegends->setCheckable(true);
    actViewSetLegends->setChecked(FALSE);
    actShowDataLess->setCheckable(true);
    actShowDataLess->setChecked(FALSE);
    actShowHidden->setCheckable(true);
    actShowHidden->setChecked(TRUE);

    /*mnuSelectorOperations->addAction(actViewIcons);
    mnuSelectorOperations->addAction(actViewSetComments);
    mnuSelectorOperations->addAction(actViewSetLegends);
    mnuSelectorOperations->addAction(actShowDataLess);
    mnuSelectorOperations->addAction(actShowHidden);*/
    mnuDisplay->addAction(actViewIcons);
    mnuDisplay->addAction(actViewSetComments);
    mnuDisplay->addAction(actViewSetLegends);
    mnuDisplay->addAction(actShowDataLess);
    mnuDisplay->addAction(actShowHidden);

    //mnuSelectorOperations->addSeparator();
    mnuSelectorOperations->addAction(actSelectAll);
    mnuSelectorOperations->addAction(actUnselectAll);
    mnuSelectorOperations->addAction(actInvertSelection);
    mnuSelectorOperations->addAction(actSelectEven);
    mnuSelectorOperations->addAction(actSelectOdd);
    mnuSelectorOperations->addAction(actSelectNth);
    mnuSelectorOperations->addAction(actSelectVisible);
    mnuSelectorOperations->addAction(actSelectInvisible);
    mnuSelectorOperations->addSeparator();
    mnuSelectorOperations->addAction(actUpdate);

    mnuColor=new QMenu(tr("Set line color"),this);
    mnuColor->setTearOffEnabled(FALSE);
    mnuLineStyle=new QMenu(tr("Set linestyle"),this);
    mnuLineStyle->setTearOffEnabled(FALSE);
    mnuSymbol=new QMenu(tr("Set set symbols"),this);
    mnuSymbol->setTearOffEnabled(FALSE);

    actColors=NULL;
    actLineStyles=NULL;
    actSymbols=NULL;
    nr_of_linestyles=nr_of_symbols=nr_of_col_items=0;
    maincolors=NULL;

    colMapper=new QSignalMapper(this);
    connect(colMapper,SIGNAL(mapped(int)),this,SLOT(colorClicked(int)));
    generateColorMenu();

    lineStyleMapper=new QSignalMapper(this);
    connect(lineStyleMapper,SIGNAL(mapped(int)),this,SLOT(lineStyleClicked(int)));
    generateLinesMenu();

    symbolMapper=new QSignalMapper(this);
    connect(symbolMapper,SIGNAL(mapped(int)),this,SLOT(symbolsClicked(int)));
    generateSymbolsMenu();

    addMenu(mnuColor);
    addMenu(mnuLineStyle);
    addMenu(mnuSymbol);
    addSeparator();
    addMenu(mnuDisplay);
    addMenu(mnuSelectorOperations);

    mnuSimpleOperations=new QMenu(tr("Set operations"),this);
    mnuSimpleOperations->setTearOffEnabled(FALSE);
    mnuSimpleOperations->addAction(actSimpleSortXUp);
    mnuSimpleOperations->addAction(actSimpleSortXDown);
    mnuSimpleOperations->addAction(actSimpleReverse);
    mnuSimpleOperations->addAction(actSimpleJoin);
    mnuSimpleOperations->addAction(actSimpleRestrict);
    mnuSimpleOperations->addAction(actSimpleSwapXY);
    addMenu(mnuSimpleOperations);
}

SetPopup::~SetPopup()
{
    if (selected_sets)
    delete[] selected_sets;
}

void SetPopup::CreateActions(void)
{
    actLabel=new QAction(tr("S0"),this);
    connect(actLabel,SIGNAL(triggered()), this, SLOT(doNothing()));
    actHide=new QAction(tr("Hide"),this);
    connect(actHide,SIGNAL(triggered()), this,SLOT(doHide()));
    actBringToFront=new QAction(tr("Bring to front"),this);
    connect(actBringToFront,SIGNAL(triggered()), this,SLOT(doBringToFront()));
    actSendToBack=new QAction(tr("Send to back"),this);
    connect(actSendToBack,SIGNAL(triggered()), this,SLOT(doSendToBack()));
    actPackAllSets=new QAction(tr("Pack all sets"),this);
    connect(actPackAllSets,SIGNAL(triggered()), this,SLOT(doPackAllSets()));
    actShow=new QAction(tr("Show"),this);
    connect(actShow,SIGNAL(triggered()), this,SLOT(doShow()));
    actDuplicate=new QAction(tr("Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()), this,SLOT(doDuplicate()));
    actKill=new QAction(tr("Kill"),this);
    connect(actKill,SIGNAL(triggered()), this,SLOT(doKill()));
    actKillData=new QAction(tr("Kill data"),this);
    connect(actKillData,SIGNAL(triggered()), this,SLOT(doKillData()));
    actCopy12=new QAction(tr("Copy S0 to S1"),this);
    connect(actCopy12,SIGNAL(triggered()), this,SLOT(doCopy12()));
    actCopy21=new QAction(tr("Copy S1 to S0"),this);
    connect(actCopy21,SIGNAL(triggered()), this,SLOT(doCopy21()));
    actMove12=new QAction(tr("Move S0 to S1"),this);
    connect(actMove12,SIGNAL(triggered()), this,SLOT(doMove12()));
    actMove21=new QAction(tr("Move S1 to S0"),this);
    connect(actMove21,SIGNAL(triggered()), this,SLOT(doMove21()));
    actSwap=new QAction(tr("Swap"),this);
    connect(actSwap,SIGNAL(triggered()), this,SLOT(doSwap()));
    actCreateByFormula=new QAction(tr("By formula"),this);
    connect(actCreateByFormula,SIGNAL(triggered()), this,SLOT(doCreateByFormula()));
    actCreateInSpreadsheet=new QAction(tr("In spreadsheet"),this);
    connect(actCreateInSpreadsheet,SIGNAL(triggered()), this,SLOT(doCreateInSpreadsheet()));
    actCreateInTextEditor=new QAction(tr("In text Editor"),this);
    connect(actCreateInTextEditor,SIGNAL(triggered()), this,SLOT(doCreateInTextEditor()));
    actCreateFromBlockData=new QAction(tr("From block data"),this);
    connect(actCreateFromBlockData,SIGNAL(triggered()), this,SLOT(doCreateFromBlockData()));
    actEditInSpreadsheet=new QAction(tr("In spreadsheet"),this);
    connect(actEditInSpreadsheet,SIGNAL(triggered()), this,SLOT(doEditInSpreadSheet()));
    actEditInTextEditor=new QAction(tr("In text editor"),this);
    connect(actEditInTextEditor,SIGNAL(triggered()), this,SLOT(doEditInTextEditor()));
    actViewSetComments=new QAction(tr("View set comments"),this);
    connect(actViewSetComments,SIGNAL(triggered()), this,SLOT(doViewSetComments()));
    actViewSetLegends=new QAction(tr("View set legends"),this);
    connect(actViewSetLegends,SIGNAL(triggered()), this,SLOT(doViewSetLegends()));
    actViewIcons=new QAction(tr("Show Icons"),this);
    connect(actViewIcons,SIGNAL(triggered()), this,SLOT(doViewIcons()));
    actShowDataLess=new QAction(tr("Show data-less"),this);
    connect(actShowDataLess,SIGNAL(triggered()), this,SLOT(doShowDataLess()));
    actShowHidden=new QAction(tr("Show hidden"),this);
    connect(actShowHidden,SIGNAL(triggered()), this,SLOT(doShowHidden()));
    actSelectAll=new QAction(tr("Select all"),this);
    connect(actSelectAll,SIGNAL(triggered()), this,SLOT(doSelectAll()));
    actUnselectAll=new QAction(tr("Unselect all"),this);
    connect(actUnselectAll,SIGNAL(triggered()), this,SLOT(doUnSelectAll()));
    actInvertSelection=new QAction(tr("Invert selection"),this);
    connect(actInvertSelection,SIGNAL(triggered()), this,SLOT(doInvertSelection()));
    actSelectEven=new QAction(tr("Select even"),this);
    connect(actSelectEven,SIGNAL(triggered()), this,SLOT(doSelectEven()));
    actSelectOdd=new QAction(tr("Select odd"),this);
    connect(actSelectOdd,SIGNAL(triggered()), this,SLOT(doSelectOdd()));
    actSelectVisible=new QAction(tr("Select visible"),this);
    connect(actSelectVisible,SIGNAL(triggered()), this,SLOT(doSelectVisible()));
    actSelectInvisible=new QAction(tr("Select invisible"),this);
    connect(actSelectInvisible,SIGNAL(triggered()), this,SLOT(doSelectInVisible()));
    actSelectNth=new QAction(tr("Select n-th (from n0)"),this);
    connect(actSelectNth,SIGNAL(triggered()), this,SLOT(doSelectNth()));
    actUpdate=new QAction(tr("Update"),this);
    connect(actUpdate,SIGNAL(triggered()), this,SLOT(doUpdate()));
    actCopyClipBoard=new QAction(tr("Copy"),this);
    connect(actCopyClipBoard,SIGNAL(triggered()),SLOT(doCopyClipBoard()));
    actPasteClipBoard=new QAction(tr("Paste"),this);
    connect(actPasteClipBoard,SIGNAL(triggered()),SLOT(doPasteClipBoard()));
    actStoreAppearance=new QAction(tr("Store appearrance(s)"),this);
    connect(actStoreAppearance,SIGNAL(triggered()),SLOT(doStoreAppearance()));
    actApplyStoredAppearance=new QAction(tr("Apply stored appearance(s)"),this);
    connect(actApplyStoredAppearance,SIGNAL(triggered()),SLOT(doApplyStoredAppearance()));

    actSimpleSortXUp=new QAction(tr("Sort X ascending"),this);
    connect(actSimpleSortXUp,SIGNAL(triggered()),SLOT(doSimpleSortXUp()));
    actSimpleSortXDown=new QAction(tr("Sort X descending"),this);
    connect(actSimpleSortXDown,SIGNAL(triggered()),SLOT(doSimpleSortXDown()));
    actSimpleReverse=new QAction(tr("Reverse X"),this);
    connect(actSimpleReverse,SIGNAL(triggered()),SLOT(doSimpleReverse()));
    actSimpleJoin=new QAction(tr("Join sets"),this);
    connect(actSimpleJoin,SIGNAL(triggered()),SLOT(doSimpleJoin()));
    actSimpleRestrict=new QAction(tr("Restrict to inside graph"),this);
    connect(actSimpleRestrict,SIGNAL(triggered()),SLOT(doSimpleRestrict()));
    actSimpleSwapXY=new QAction(tr("Swap X with Y"),this);
    connect(actSimpleSwapXY,SIGNAL(triggered()),SLOT(doSimpleSwapXY()));

    /*actSyncListContents=new QAction(tr("Sync source and destination"),this);
    connect(actSyncListContents,SIGNAL(triggered()),SLOT(doSyncList()));*/
}

void SetPopup::prepareForAction(void)
{
//qDebug() << "A Prepare for Action gno=" << selected_graph << " nr_sel_sets=" << number_of_selected_sets;
    if (par!=NULL)
    {
    selected_graph=par->gr_no;
    par->get_selection(&number_of_selected_sets,&selected_sets);
    }
    else
    {
    number_of_selected_sets=1;
    selected_graph=main_win_gr_no;
    if (selected_sets!=NULL) delete[] selected_sets;
    selected_sets=new int[2];
    selected_sets[0]=main_win_set_no;
    }
//qDebug() << "B Prepare for Action gno=" << selected_graph << " nr_sel_sets=" << number_of_selected_sets;
}

void SetPopup::finishAction(int preserveSelection)
{
//qDebug() << "Finish Action";
    bool savUpRun=updateRunning;
    updateRunning=false;
    force_redraw();
    updateRunning=savUpRun;
    hide();/// we should not need this
    if (par!=NULL)
    {
    par->update_number_of_entries();
        if (preserveSelection==DO_PRESERVE_SELECTION)
        par->set_new_selection(number_of_selected_sets,selected_sets);
    }
    else
    {
    mainWin->stop_highlighting();
    drawgraph();
    }
}

void SetPopup::doNothing(void)
{
//qDebug() << "Nothing";
    if (par!=NULL)
    {
    par->activateHighlighting();
    }
    else
    {
    ;//mainWin->start_highlighting();//should be highlighted anyway
    }
}

void SetPopup::doHide(void)
{
    prepareForAction();
    ListOfChanges.clear();
    ListOfOldStates.clear();
    int * gnos=new int[number_of_selected_sets];
    int * snos=new int[number_of_selected_sets];
    sprintf(dummy,"with g%d",selected_graph);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gnos[i]=selected_graph;
        snos[i]=selected_sets[i];
        sprintf(dummy,"    s%d hidden true",selected_sets[i]);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    s%d hidden false",selected_sets[i]);
        ListOfOldStates << QString(dummy);
        set_set_hidden(selected_graph, selected_sets[i], TRUE);
    }
    ///Undo-Stuff
    ShowHideSets(number_of_selected_sets,gnos,snos);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    delete[] gnos;
    delete[] snos;
    finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doShow(void)
{
    prepareForAction();
    ListOfChanges.clear();
    ListOfOldStates.clear();
    int * gnos=new int[number_of_selected_sets];
    int * snos=new int[number_of_selected_sets];
    sprintf(dummy,"with g%d",selected_graph);
    ListOfChanges << QString(dummy);
    ListOfOldStates << QString(dummy);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gnos[i]=selected_graph;
        snos[i]=selected_sets[i];
        sprintf(dummy,"    s%d hidden false",selected_sets[i]);
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    s%d hidden true",selected_sets[i]);
        ListOfOldStates << QString(dummy);
        set_set_hidden(selected_graph, selected_sets[i], FALSE);
    }
    ///Undo-Stuff
    ShowHideSets(number_of_selected_sets,gnos,snos);
    ListOfChanges.clear();
    ListOfOldStates.clear();
    delete[] gnos;
    delete[] snos;
    finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doSendToBack(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
    for (int i=number_of_selected_sets-1;i>=0;i--)
    {
        ChangeSetOrdering(selected_graph,selected_sets[i],PUSH_SET_TOBACK);
        pushset(selected_graph, selected_sets[i], PUSH_SET_TOBACK);
    }
    mainWin->mainArea->completeRedraw();
    if (par!=NULL)
    {
    int *n_gnos=new int[number_of_selected_sets+1];
    for (int i=0;i<number_of_selected_sets;i++) n_gnos[i]=i;
    par->set_new_selection(number_of_selected_sets,n_gnos);
    delete[] n_gnos;
    par->new_selection();
    }
}

void SetPopup::doBringToFront(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
    for (int i=0;i<number_of_selected_sets;i++)
    {
        ChangeSetOrdering(selected_graph,selected_sets[i],PUSH_SET_TOFRONT);
        pushset(selected_graph, selected_sets[i], PUSH_SET_TOFRONT);
    }
    mainWin->mainArea->completeRedraw();
    if (par!=NULL)
    {
    int *n_gnos=new int[number_of_selected_sets+1];
    for (int i=0;i<number_of_selected_sets;i++) n_gnos[i]=new_set_no-i;
    par->set_new_selection(number_of_selected_sets,n_gnos);
    delete[] n_gnos;
    par->new_selection();
    }
}

void SetPopup::doDuplicate(void)
{
int setno,index;
    prepareForAction();
    if (number_of_selected_sets<1) return;
int * gnos=new int[number_of_selected_sets+2];
int * snos=new int[number_of_selected_sets+2];
    index=0;
    for (int i=0;i<number_of_selected_sets;i++)
    {
        setno = nextset(selected_graph);
        do_copyset(selected_graph, selected_sets[i], selected_graph, setno);
        gnos[index]=selected_graph;
        snos[index++]=setno;
    }
    SetsCreated(index,gnos,snos,UNDO_COMPLETE);
delete[] gnos;
delete[] snos;
/*    mainWin->mainArea->completeRedraw();
if (par!=NULL)
par->set_new_selection(number_of_selected_sets,selected_sets);*/
finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doKill(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
    if (yesno(tr("Kill selected set(s)?").toLocal8Bit().data(), NULL, NULL, NULL))
    {
        ListOfChanges.clear();
        int * gnos=new int[number_of_selected_sets];
        for (int i=0;i<number_of_selected_sets;i++)
        {
            sprintf(dummy,"kill G%d.S%d",selected_graph,selected_sets[i]);
            ListOfChanges << QString(dummy);
            gnos[i]=selected_graph;
        }
        SetsDeleted(number_of_selected_sets,gnos,selected_sets,UNDO_COMPLETE);//UndoStuff
        delete[] gnos;
        for (int i=0;i<number_of_selected_sets;i++)
        {
            killset(selected_graph, selected_sets[i]);
        }
        if (autoPackSets)
        {
            doPackAllSets();
        }
        finishAction(DO_NOT_PRESERVE_SELECTION);
        ListOfChanges.clear();
    }
    else
        finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doKillData(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
    if (yesno("Kill data in selected set(s)?", NULL, NULL, NULL))
    {
        int * gnos=new int[number_of_selected_sets];
        for (int i=0;i<number_of_selected_sets;i++)
            gnos[i]=selected_graph;
        SetsDeleted(number_of_selected_sets,gnos,selected_sets,UNDO_COMPLETE);//UndoStuff
        char dummy[128];
        sprintf(dummy,"[G%d.S%d]",gnos[0],selected_sets[0]);
        if (number_of_selected_sets>1)
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Kill set data"),QString(),-1);
        }
        else
        {
            addAditionalDescriptionToLastNode(-1,QObject::tr("Kill set data")+QString(dummy),QString(),-1);
        }
        delete[] gnos;
        for (int i=0;i<number_of_selected_sets;i++)
        {
            killsetdata(selected_graph, selected_sets[i]);
        }
        finishAction(DO_NOT_PRESERVE_SELECTION);
    }
    else
        finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doSwap(void)
{
    prepareForAction();
    if (number_of_selected_sets==2)
    {
        swapset(selected_graph, selected_sets[0], selected_graph, selected_sets[1]);
        UndoSwapSets(selected_graph, selected_sets[0], selected_graph, selected_sets[1]);
    }
        if (par!=NULL)
        par->update_number_of_entries();
finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doPackAllSets(void)
{
    prepareForAction();
    SetsAboutToBePacked(selected_graph);
    packsets(selected_graph);
        if (par!=NULL)
        par->update_number_of_entries();
    //mainWin->mainArea->completeRedraw();
    finishAction(DO_PRESERVE_SELECTION);
}

void SetPopup::doEditInSpreadSheet(void)
{
    prepareForAction();
    for (int i=0;i<number_of_selected_sets;i++)
    {
        showSetInSpreadSheet(selected_graph,selected_sets[i]);
    }
}

void SetPopup::doEditInTextEditor(void)
{
    prepareForAction();
    int gno=this->selected_graph;
    int setno=-1;
    if (this->number_of_selected_sets>0)
    {
        setno=this->selected_sets[0];
    }
    if (setno!= -1)
    {
        setcomment(gno, setno,tr("Editor").toLocal8Bit().data());
        set_set_hidden(gno, setno, FALSE);
        if (FormSetEditor==NULL)
        {
            FormSetEditor=new frmSetEditor(0);
        }
        FormSetEditor->init(gno,setno);
        FormSetEditor->show();
        FormSetEditor->raise();
        FormSetEditor->activateWindow();
        ///FormSetEditor->setModal(true);
    }
}

void SetPopup::doCreateByFormula(void)
{
    if (FormLoadAndEvaluate==NULL)
    {
        FormLoadAndEvaluate=new frmLoadEval(0);
    }
    FormLoadAndEvaluate->parentList=this->par;
    FormLoadAndEvaluate->show();
    FormLoadAndEvaluate->raise();
    FormLoadAndEvaluate->activateWindow();
}

void SetPopup::doCreateInSpreadsheet(void)
{
    prepareForAction();
    int setno,gno=selected_graph;
    if ((setno = nextset(gno)) != -1) {
        setcomment(gno, setno, tr("Editor").toLocal8Bit().data());
        set_set_hidden(gno, setno, FALSE);
        //create_ss_frame(gno, setno);
        showSetInSpreadSheet(gno,setno);
    frmSpreadSheet * foundSpreadsheet=findOpenSpreadSheet(gno,setno);
        if (foundSpreadsheet!=NULL)
        foundSpreadsheet->parentList=this->par;
    } else {
        ;///cout << "error" << endl;
    }
}

void SetPopup::doCreateInTextEditor(void)
{
    prepareForAction();
    int gno=this->selected_graph;
    int setno;
    if ((setno = nextset(gno)) != -1)
    {
        setcomment(gno, setno, "Editor");
        set_set_hidden(gno, setno, FALSE);
        if (FormSetEditor==NULL)
        {
            FormSetEditor=new frmSetEditor(0);
        }
        FormSetEditor->init(gno,setno);
        FormSetEditor->show();
        FormSetEditor->raise();
        FormSetEditor->activateWindow();
        ///FormSetEditor->setModal(true);
    }
}

void SetPopup::doCreateFromBlockData(void)
{
    if (FormEditBlockData==NULL)
    {
        FormEditBlockData=new frmEditBlockData(mainWin);
    }
    FormEditBlockData->init();
    int blockncols = get_blockncols();
    if (!blockncols)
        FormEditBlockData->hide();
    else
    {
        FormEditBlockData->show();
        FormEditBlockData->raise();
        FormEditBlockData->activateWindow();
    }
}

void SetPopup::doSelectAll(void)
{
    if (par!=NULL)
    par->selectAll();
}

void SetPopup::doUnSelectAll(void)
{
    if (par!=NULL)
    par->clearSelection();
}

void SetPopup::doSelectEven(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (par->entries[i]%2==0 || in_list==true)//index is even
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] ne_sel;
}

void SetPopup::doSelectOdd(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (par->entries[i]%2!=0 || in_list==true)//index is odd
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] ne_sel;
}

void SetPopup::doSelectVisible(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (is_set_hidden(par->gr_no,par->entries[i])==FALSE || in_list==true)//set is visible
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] ne_sel;
}

void SetPopup::doSelectInVisible(void)
{
    if (par==NULL) return;
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if (is_set_hidden(par->gr_no,par->entries[i])==TRUE || in_list==true)//set is visible
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] ne_sel;
}

void SetPopup::doSelectNth(void)
{
    if (par==NULL) return;
bool ok;
int	n=QInputDialog::getInt(this,tr("Index"),tr("n="),2,0,par->number_of_entries,1,&ok,0);
if (ok==false) return;
int offset=QInputDialog::getInt(this,tr("Offset"),tr("Start index n0="),0,0,par->number_of_entries,1,&ok,0);
if (ok==false) return;
if (n==0 || n==1)
{
doSelectAll();
return;
}
QModelIndexList list=par->selectedIndexes();
int number=list.size();
    int * ne_sel=new int[par->count()+2];
    int counter=0;
    bool in_list;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;
        in_list=false;
        for (int j=0;j<number;j++)
        {
            if (list.at(j).row()==i)
            {
            in_list=true;
            break;
            }
        }
        if ((par->entries[i]+n-1-offset)%n==n-1 || in_list==true)
        {
            ne_sel[counter++]=par->entries[i];
        }
    }
    par->set_new_selection(counter,ne_sel);
    delete[] ne_sel;
}

void SetPopup::doUpdate(void)
{
    if (par==NULL) return;
    par->update_number_of_entries_preserve_selection();
}

void SetPopup::doInvertSelection(void)
{
    if (par==NULL) return;
    int number=par->count()-number_of_selected_sets;
    int * n_selected_sets=new int[number+2];
    bool in_list;
    in_list=false;
    for (int i=0;i<number_of_selected_sets;i++)//check for the "All sets"-entry
    {
        if (selected_sets[i]==-1)
        {
        in_list=true;
        break;
        }
    }
    if (in_list==true)//all-sets-entry is selected --> clear selection
    {
    par->clearSelection();
    }
    else
    {
        number=0;
    for (int i=0;i<par->count();i++)
    {
        if (par->entries[i]==-1) continue;//ignore "All sets"-entry
        in_list=false;
        for (int j=0;j<number_of_selected_sets;j++)
        {
            if (selected_sets[j]==par->entries[i])
            {
                in_list=true;
                break;
            }
        }
        if (in_list==false)
        {
            n_selected_sets[number++]=par->entries[i];
        }
    }
    par->set_new_selection(number,n_selected_sets);
    }
    delete[] n_selected_sets;
}

void SetPopup::doShowHidden(void)
{
    if (par!=NULL)
    {
    par->show_hidden=!par->show_hidden;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doShowDataLess(void)
{
    if (par!=NULL)
    {
    par->show_data_less=!par->show_data_less;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doViewSetComments(void)
{
    if (par!=NULL)
    {
    par->show_comments=!par->show_comments;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doViewSetLegends(void)
{
    if (par!=NULL)
    {
    par->show_legends=!par->show_legends;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doViewIcons(void)
{
    if (par!=NULL)
    {
    par->show_icons=!par->show_icons;
    par->update_number_of_entries_preserve_selection();
    }
}

void SetPopup::doMove12(void)
{
    if (number_of_selected_sets==2 && par!=NULL)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Replace S%d?", selected_sets[1]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            moveset(gno, selected_sets[0], gno, selected_sets[1]);
            SetsModified(1,gnos,selected_sets+1,UNDO_COMPLETE);
            sprintf(dummy,"Move [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[0], gno, selected_sets[1]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doMove21(void)
{
    if (number_of_selected_sets==2 && par!=NULL)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Replace S%d?", selected_sets[0]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            moveset(gno, selected_sets[1], gno, selected_sets[0]);
            //selected_sets[0]=selected_sets[1];
            SetsModified(1,gnos,selected_sets,UNDO_COMPLETE);
            sprintf(dummy,"Move [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[1], gno, selected_sets[0]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doCopy12(void)
{
    if (number_of_selected_sets==2 && par!=NULL)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Overwrite S%d?", selected_sets[1]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            do_copyset(gno, selected_sets[0], gno, selected_sets[1]);
            SetsModified(2,gnos,selected_sets,UNDO_COMPLETE);
            sprintf(dummy,"Copy [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[0], gno, selected_sets[1]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doCopy21(void)
{
    if (number_of_selected_sets==2 && par!=NULL)
    {
        int gno=par->gr_no;
        int * gnos=new int[2];
        gnos[0]=gnos[1]=gno;
        char dummy[64];
        sprintf(dummy, "Overwrite S%d?", selected_sets[0]);
        if (yesno(dummy, NULL, NULL, NULL))
        {
            SaveSetStatesPrevious(2,gnos,selected_sets,UNDO_COMPLETE);
            do_copyset(gno, selected_sets[1], gno, selected_sets[0]);
            SetsModified(2,gnos,selected_sets,UNDO_COMPLETE);
            sprintf(dummy,"Copy [G%d.S%d]-->[G%d.S%d]",gno, selected_sets[1], gno, selected_sets[0]);
            addAditionalDescriptionToLastNode(-1,QString(dummy),QString(),0);
        }
        delete[] gnos;
        finishAction(DO_PRESERVE_SELECTION);
    }
}

void SetPopup::doCopyClipBoard(void)
{
    prepareForAction();
    if (number_of_selected_sets<1) return;
    QString text;
    text.clear();
    char dummy[2048];
    int gno,sno,col;
    for (int i=0;i<number_of_selected_sets;i++)
    {
        gno=selected_graph;
        sno=selected_sets[i];
        col=dataset_cols(gno,sno);
        for (int k=0;k<g[gno].p[sno].data.len;k++)
        {
            for (int j=0;j<col;j++)
            {
                //sprintf(dummy,"%.8g\t",g[gno].p[sno].data.ex[j][k]);
                //text+=QString(dummy);
                sprintf(dummy,sformat,g[gno].p[sno].data.ex[j][k]);
                text+=QString(dummy);
                if (j<col-1)
                text+=QString("\t");
            }
            text+=QString("\n");
        }
        text+=QString("\n");
    }
    if (SystemsDecimalPoint!='.') text.replace(QString("."),QString(SystemsDecimalPoint));
    QApplication::clipboard()->setText(text);
}

void SetPopup::doPasteClipBoard(void)
{
    GeneralPaste(QApplication::clipboard()->mimeData());
}

void SetPopup::doStoreAppearance(void)
{
//at first: deleted already saved appearances
    bool ddssm=dont_delete_saved_set_memory;
    dont_delete_saved_set_memory=true;
    for (int i=0;i<nr_of_set_app_saved;i++)
    deleteSavedSet(saved_set_app+i,UNDO_APPEARANCE);
    dont_delete_saved_set_memory=ddssm;
//find out how many sets have been selected
prepareForAction();
//prepare space for storage
    if (number_of_selected_sets>nr_of_set_app_allocated)
    {
        if (saved_set_app!=NULL) delete[] saved_set_app;
        saved_set_app=new plotarr[number_of_selected_sets];
        nr_of_set_app_allocated=number_of_selected_sets;
    }
//store the appearance
    for (int i=0;i<number_of_selected_sets;i++)
    {
    copySet(selected_graph,selected_sets[i],saved_set_app+i,UNDO_APPEARANCE);
    }
    nr_of_set_app_saved=number_of_selected_sets;
}

void reset_set_appearance_only(int gno, int sno, plotarr * pn)
{
if (is_valid_setno(gno,sno)==FALSE) return;
plotarr * po=g[gno].p+sno;
po->sym=pn->sym;
po->symsize=pn->symsize;
po->symlines=pn->symlines;
po->symlinew=pn->symlinew;
po->symskip=pn->symskip;
po->symchar=pn->symchar;
po->charfont=pn->charfont;
po->linet=pn->linet;
po->lines=pn->lines;
po->linew=pn->linew;
po->baseline_type=pn->baseline_type;
po->baseline=pn->baseline;
po->dropline=pn->dropline;
po->filltype=pn->filltype;
po->polygone_base_set=pn->polygone_base_set;
po->fillrule=pn->fillrule;
po->sympen=pn->sympen;
po->symfillpen=pn->symfillpen;
po->linepen=pn->linepen;
po->setfillpen=pn->setfillpen;
memcpy(&(po->avalue),&(pn->avalue),sizeof(AValue));
memcpy(&(po->errbar),&(pn->errbar),sizeof(Errbar));
}

void SetPopup::doApplyStoredAppearance(void)
{
    prepareForAction();
if (nr_of_set_app_saved<=0 || number_of_selected_sets<=0) return;

int * selected_graphs=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) selected_graphs[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,selected_graphs,selected_sets,UNDO_APPEARANCE);
    /*if (nr_of_set_app_saved==1)//one appearance save, many sets selected
    {
        for (int i=0;i<number_of_selected_sets;i++)
        {
        reset_set_appearance_only(selected_graph,selected_sets[i],saved_set_app[0]);
        }
    }
    else
    {*/
int counter=0;
for (int i=0;i<number_of_selected_sets;i++)
{
reset_set_appearance_only(selected_graph,selected_sets[i],saved_set_app+counter);
counter++;
if (counter>=nr_of_set_app_saved) counter=0;
}
    //}
ListOfChanges.clear();
ListOfOldStates.clear();
SetsModified(number_of_selected_sets,selected_graphs,selected_sets,UNDO_APPEARANCE);
delete[] selected_graphs;
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleSortXUp(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_SORT,DATA_X,0,1);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleSortXDown(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_SORT,DATA_X,1,1);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleReverse(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_REVERSE,0,0,0);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleJoin(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_JOIN,0,0,0);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleRestrict(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_RESTRICT,RESTRICT_WORLD,0,0);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::doSimpleSwapXY(void)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
int ret=SmallSetOperations(number_of_selected_sets,sel_gr,selected_sets,DATASETOP_SWAP_COLS,0,1,0);
if (sel_gr!=NULL) delete[] sel_gr;
if (ret==RETURN_SUCCESS)
mainWin->mainArea->completeRedraw();
}

void SetPopup::generateColorMenu(void)
{
prepareForAction();
    if (actColors!=NULL)//delete old entries
    {
        for (int i=0;i<nr_of_col_items;i++)
        {
        mnuColor->removeAction(actColors[i]);
        delete actColors[i];
        }
        delete[] actColors;
    }
int nr_of_aux_cols;
if (maincolors)
{
delete[] maincolors;
maincolors=NULL;
}
nr_of_col_items=get_main_color_indices(&maincolors,&nr_of_aux_cols);
//qDebug() << "cur number of colors=" << nr_of_col_items;
actColors=new QAction*[nr_of_col_items+2];
QIcon tmp_icon;
    for (int i=0;i<nr_of_col_items;i++)
    {
    tmp_icon=createSingleSmallColorIcon(i);
        if (cmap_table[i].cname)
        actColors[i]=new QAction(tmp_icon,QString(cmap_table[i].cname),this);
        else
        actColors[i]=new QAction(tmp_icon,tr("unnamed"),this);
    actColors[i]->setCheckable(true);
    connect(actColors[i],SIGNAL(triggered()),colMapper,SLOT(map()));
        if (number_of_selected_sets>0 && is_valid_setno(selected_graph,selected_sets[0]))
        {
//qDebug() << "i=" << i << " selected_graph=" << selected_graph << " sel_set_nr=" << selected_sets[0];
            if (i==g[selected_graph].p[selected_sets[0]].linepen.color)
            actColors[i]->setChecked(true);
            else
            actColors[i]->setChecked(false);
        }
        else
        {
        actColors[i]->setChecked(false);
        }
    mnuColor->addAction( actColors[i] );
    colMapper->setMapping(actColors[i],i);
    }
mnuColor->update();
}

void SetPopup::generateLinesMenu(void)
{
prepareForAction();
    if (actLineStyles!=NULL)//delete old entries
    {
        for (int i=0;i<nr_of_linestyles;i++)
        {
        mnuLineStyle->removeAction(actLineStyles[i]);
        delete actLineStyles[i];
        }
        delete[] actLineStyles;
    }
nr_of_linestyles=nr_of_current_linestyles;
//qDebug() << "cur number of linestyles=" << nr_of_current_linestyles;
actLineStyles=new QAction*[nr_of_current_linestyles];
QString * linest_names=new QString[nr_of_current_linestyles];
linest_names[0]=tr("None");
linest_names[1]=tr("straight");
linest_names[2]=tr("narrow slashed");
linest_names[3]=tr("medium slashed");
linest_names[4]=tr("wide slashed");
linest_names[5]=tr("slash dot");
linest_names[6]=tr("wide slash dot");
linest_names[7]=tr("slash dot dot");
linest_names[8]=tr("slash slash dot");
for (int i=9;i<nr_of_current_linestyles;i++)
linest_names[i]=QString("");
QIcon tmp_icon;
    for (int i=0;i<nr_of_current_linestyles;i++)
    {
    //tmp_icon=QIcon(*LinePixmaps[i]);
    tmp_icon=createSingleSmallLineStyleIcon(i);
    actLineStyles[i]=new QAction(tmp_icon,linest_names[i],this);
    actLineStyles[i]->setCheckable(true);
    connect(actLineStyles[i],SIGNAL(triggered()),lineStyleMapper,SLOT(map()));
        if (number_of_selected_sets>0 && is_valid_setno(selected_graph,selected_sets[0]))
        {
            if (i==g[selected_graph].p[selected_sets[0]].lines)
            actLineStyles[i]->setChecked(true);
            else
            actLineStyles[i]->setChecked(false);
        }
        else
        {
        actLineStyles[i]->setChecked(false);
        }
    mnuLineStyle->addAction( actLineStyles[i] );
    lineStyleMapper->setMapping( actLineStyles[i], i );
    }
delete[] linest_names;
mnuLineStyle->update();
}

void SetPopup::generateSymbolsMenu(void)
{
///nr_of_symbols
prepareForAction();
    if (actSymbols!=NULL)//delete old entries
    {
        for (int i=0;i<nr_of_symbols;i++)
        {
        mnuColor->removeAction(actSymbols[i]);
        delete actSymbols[i];
        }
        delete[] actSymbols;
    }
nr_of_symbols=11;
//qDebug() << "cur number of colors=" << nr_of_symbols;
    actSymbols=new QAction*[nr_of_symbols];
/* symbol-icons */
QString entr[11];
    entr[0]=tr("None");
    entr[1]=tr("Circle");
    entr[2]=tr("Square");
    entr[3]=tr("Diamond");
    entr[4]=tr("Triangle up");
    entr[5]=tr("Triangle left");
    entr[6]=tr("Triangle down");
    entr[7]=tr("Triangle right");
    entr[8]=tr("Plus");
    entr[9]=tr("X");
    entr[10]=tr("Star");
QIcon tmp_icon;
QPixmap map1(12,12);
QPainter paint1(&map1);
paint1.setBrush(Qt::white);
paint1.setPen(Qt::white);
paint1.drawRect(0,0,13,13);
paint1.setPen(Qt::black);
paint1.setBrush(Qt::black);
paint1.end();
tmp_icon=QIcon(map1);
    for (int i=0;i<nr_of_symbols;i++)
    {
        if (i!=0)
        {
        paint1.begin(&map1);
        paint1.setBrush(Qt::white);
        paint1.setPen(Qt::white);
        paint1.drawRect(0,0,13,13);
        paint1.setPen(Qt::black);
        paint1.setBrush(Qt::black);
            drawSimpleSymbol(paint1,6,6,10,i,65);
        paint1.end();
        tmp_icon=QIcon(map1);
        }
    actSymbols[i]=new QAction(tmp_icon,entr[i],this);
    actSymbols[i]->setCheckable(true);
    connect(actSymbols[i],SIGNAL(triggered()),symbolMapper,SLOT(map()));
        if (number_of_selected_sets>0 && is_valid_setno(selected_graph,selected_sets[0]))
        {
            if (i==g[selected_graph].p[selected_sets[0]].sym)
            actSymbols[i]->setChecked(true);
            else
            actSymbols[i]->setChecked(false);
        }
        else
        {
        actSymbols[i]->setChecked(false);
        }
    mnuSymbol->addAction( actSymbols[i] );
    symbolMapper->setMapping(actSymbols[i],i);
    }
mnuSymbol->update();
}

void SetPopup::colorClicked(int c)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        if (!is_valid_setno(sel_gr[i],selected_sets[i])) continue;
        g[sel_gr[i]].p[selected_sets[i]].linepen.color=c;
    }
SetsModified(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
mainWin->mainArea->completeRedraw();
if (number_of_selected_sets>0)
{
    if (par!=NULL)
    par->set_new_selection(number_of_selected_sets,selected_sets);
updateSetAppearance(sel_gr[0],selected_sets[0]);
}
delete[] sel_gr;
}

void SetPopup::lineStyleClicked(int c)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
    for (int i=0;i<number_of_selected_sets;i++)
    {
        if (!is_valid_setno(sel_gr[i],selected_sets[i])) continue;
        g[sel_gr[i]].p[selected_sets[i]].lines=c;
    }
SetsModified(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
mainWin->mainArea->completeRedraw();
if (number_of_selected_sets>0)
{
    if (par!=NULL)
    par->set_new_selection(number_of_selected_sets,selected_sets);
updateSetAppearance(sel_gr[0],selected_sets[0]);
}
delete[] sel_gr;
}

void SetPopup::symbolsClicked(int c)
{
    prepareForAction();
int * sel_gr=NULL;
sel_gr=new int[number_of_selected_sets+1];
for (int i=0;i<number_of_selected_sets;i++) sel_gr[i]=selected_graph;
SaveSetStatesPrevious(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
//qDebug() << "number_of_selected_sets=" << number_of_selected_sets;
    for (int i=0;i<number_of_selected_sets;i++)
    {
//qDebug() << "G" << sel_gr[i] << ".S" << selected_sets[i];
        if (!is_valid_setno(sel_gr[i],selected_sets[i])) continue;
        g[sel_gr[i]].p[selected_sets[i]].sym=c;
    }
SetsModified(number_of_selected_sets,sel_gr,selected_sets,UNDO_APPEARANCE);
mainWin->mainArea->completeRedraw();
if (number_of_selected_sets>0)
{
    if (par!=NULL)
    par->set_new_selection(number_of_selected_sets,selected_sets);
updateSetAppearance(sel_gr[0],selected_sets[0]);
}
delete[] sel_gr;
}

/*
void SetPopup::doSyncList(void)
{
    if (par!=NULL)
    par->doSyncNow();
}
*/

void SetPopup::update_menu_content(void)
{
prepareForAction();
//qDebug() << "UPDATE MENU: gno=" << selected_graph << " nr_of_sel_sets=" << number_of_selected_sets;// <<  selected_sets[i]
    char dummy[128];
    char dummy2[128];
    if (par!=NULL)
    {
    actViewSetComments->setChecked(par->show_comments);
    actViewSetLegends->setChecked(par->show_legends);
    actViewIcons->setChecked(par->show_icons);
    actShowDataLess->setChecked(par->show_data_less);
    actShowHidden->setChecked(par->show_hidden);
        /*if (par->allowSnycCommand)
        actSyncListContents->setVisible(true);
        else
        actSyncListContents->setVisible(false);*/
    }
    /*else
    actSyncListContents->setVisible(false);*/

    if (number_of_selected_sets==0)
    {
        actLabel->setText(tr("None"));
        actHide->setEnabled(false);
        actBringToFront->setEnabled(false);
        actSendToBack->setEnabled(false);
        actPackAllSets->setEnabled(false);
        actShow->setEnabled(false);
        actDuplicate->setEnabled(false);
        actKill->setEnabled(false);
        actKillData->setEnabled(false);
        actSwap->setEnabled(false);
        actEditInSpreadsheet->setEnabled(false);
        actEditInTextEditor->setEnabled(false);
        actCopy12->setVisible(false);
        actCopy21->setVisible(false);
        actMove12->setVisible(false);
        actMove21->setVisible(false);
        actCopyClipBoard->setEnabled(false);
        /*actions()[2]->setEnabled(FALSE);
    actions()[3]->setEnabled(FALSE);
    actions()[4]->setEnabled(FALSE);
    actions()[5]->setEnabled(FALSE);
    actions()[7]->setEnabled(FALSE);
    actions()[8]->setEnabled(FALSE);
    actions()[9]->setEnabled(FALSE);
    actions()[11]->setEnabled(FALSE);
    mnuEdit->actions()[0]->setEnabled(FALSE);
    mnuEdit->actions()[1]->setEnabled(FALSE);*/
        actStoreAppearance->setEnabled(false);
        actApplyStoredAppearance->setEnabled(false);
        mnuSimpleOperations->setEnabled(false);
    }
    else//at least 1 set selected
    {
        actHide->setEnabled(true);
        actBringToFront->setEnabled(true);
        actSendToBack->setEnabled(true);
        actPackAllSets->setEnabled(true);
        actShow->setEnabled(true);
        actDuplicate->setEnabled(true);
        actKill->setEnabled(true);
        actKillData->setEnabled(true);
        actSwap->setEnabled(true);
        actEditInSpreadsheet->setEnabled(true);
        actEditInTextEditor->setEnabled(true);
        actCopyClipBoard->setEnabled(true);
        /*actions()[2]->setEnabled(TRUE);
    actions()[3]->setEnabled(TRUE);
    actions()[4]->setEnabled(TRUE);
    actions()[5]->setEnabled(TRUE);
    actions()[7]->setEnabled(TRUE);
    actions()[8]->setEnabled(TRUE);
    actions()[9]->setEnabled(TRUE);
    actions()[11]->setEnabled(TRUE);
    mnuEdit->actions()[0]->setEnabled(TRUE);
    mnuEdit->actions()[1]->setEnabled(TRUE);*/
        actStoreAppearance->setEnabled(true);
            if (nr_of_set_app_saved>0)
            actApplyStoredAppearance->setEnabled(true);
            else
            actApplyStoredAppearance->setEnabled(false);
        sprintf(dummy,"S");
//qDebug() << "update_menu_contents: nr=" << number_of_selected_sets;
        for (int i=0;i<number_of_selected_sets;i++)
        {
            sprintf(dummy2,"%d",selected_sets[i]);
            strcat(dummy,dummy2);
            if (i>2 && i!=number_of_selected_sets-1)
            {
                strcat(dummy,",...");
                break;
            }
            else
            {
                if (i!=number_of_selected_sets-1)
                    strcat(dummy,", S");
            }
        }
        actLabel->setText(QString(dummy));

        if (number_of_selected_sets==1)
        {
            actCopy12->setVisible(false);
            actCopy21->setVisible(false);
            actMove12->setVisible(false);
            actMove21->setVisible(false);
            actSwap->setEnabled(false);
            actBringToFront->setEnabled(true);
            actSendToBack->setEnabled(true);
            actEditInSpreadsheet->setEnabled(true);
            actEditInTextEditor->setEnabled(true);
            actSimpleJoin->setEnabled(false);
        }
        else
        {
            if (number_of_selected_sets==2)
            {
                sprintf(dummy,"Copy S%d to S%d",selected_sets[0],selected_sets[1]);
                actCopy12->setText(QString(dummy));
                actCopy12->setVisible(true);
                sprintf(dummy,"Copy S%d to S%d",selected_sets[1],selected_sets[0]);
                actCopy21->setText(QString(dummy));
                actCopy21->setVisible(true);
                sprintf(dummy,"Move S%d to S%d",selected_sets[0],selected_sets[1]);
                actMove12->setText(QString(dummy));
                actMove12->setVisible(true);
                sprintf(dummy,"Move S%d to S%d",selected_sets[1],selected_sets[0]);
                actMove21->setText(QString(dummy));
                actMove21->setVisible(true);
                actSwap->setEnabled(true);
            }
            else
            {
                actCopy12->setVisible(false);
                actCopy21->setVisible(false);
                actMove12->setVisible(false);
                actMove21->setVisible(false);
                actSwap->setEnabled(false);
            }
            actBringToFront->setEnabled(false);
            actSendToBack->setEnabled(false);
            actEditInSpreadsheet->setEnabled(false);
            actEditInTextEditor->setEnabled(false);
            actSimpleJoin->setEnabled(true);
        }
        mnuSimpleOperations->setEnabled(true);
    }
    if (par==NULL)
    {
    mnuDisplay->setEnabled(false);
    mnuSelectorOperations->setEnabled(false);
    mnuSimpleOperations->setEnabled(true);
    }
    else
    {
    mnuDisplay->setEnabled(true);
    mnuSelectorOperations->setEnabled(true);
    mnuSimpleOperations->setEnabled(true);
    }
    if (QApplication::clipboard()->mimeData()->hasText() || QApplication::clipboard()->mimeData()->hasUrls())
        actPasteClipBoard->setEnabled(true);
    else
        actPasteClipBoard->setEnabled(false);
    generateColorMenu();
    generateLinesMenu();
    generateSymbolsMenu();
}

void SetPopup::hideEvent(QHideEvent * e)
{
e->accept();
    if (par==NULL)
    {
    mainWin->stop_highlighting();
    }
QMenu::hideEvent(e);
}

QPen createPen(int lines,double linew,int color)
{
QPen tmp_pen;
tmp_pen.setColor(get_Color(color));
tmp_pen.setWidthF(qreal(linew));
if (lines==0)
tmp_pen.setStyle(Qt::NoPen);
else if (lines==1)
tmp_pen.setStyle(Qt::SolidLine);
else
{
tmp_pen.setStyle(Qt::CustomDashLine);
tmp_pen.setDashPattern(*PenDashPattern[lines]);
//GeneralPainter->setBackground(Qt::transparent);/// setzte beim Linienzeichnen den Hintergrund auf transparent!!
}
return tmp_pen;
}

void drawSimpleSymbol(QPainter & paint1,int x,int y,int w,int sym,char sym_c=65)//paints a symbol on a painter in the size w x w (always a square) on middle-position x,y; color and linestyle have to be set beforehand
{
int npoints=0;
int wh=w/2;
QPoint *p = new QPoint[5];
if (sym==SYM_CIRCLE)
{
paint1.drawEllipse(x-wh,y-wh,w,w);
}
else if (sym==SYM_CHAR)//draw some Text
{
paint1.drawText(x-wh*1.8,y+wh*1.8,QString(sym_c));
}
else if (sym==SYM_PLUS)//plus
{
paint1.drawLine(x-wh,y,x+wh,y);
paint1.drawLine(x,y-wh,x,y+wh);
}
else if (sym==SYM_X)//X
{
paint1.drawLine(x-wh,y-wh,x+wh,y+wh);
paint1.drawLine(x+wh,y-wh,x-wh,y+wh);
}
else if (sym==SYM_SPLAT)//star
{
paint1.drawLine(x-wh,y,x+wh,y);
paint1.drawLine(x,y-wh,x,y+wh);
if (w>8)
wh*=0.8;
paint1.drawLine(x-wh,y-wh,x+wh,y+wh);
paint1.drawLine(x+wh,y-wh,x-wh,y+wh);
}
else
{
    switch (sym)
    {
    case SYM_SQUARE://square
        p[0].setX(x-wh);
        p[0].setY(y-wh);
        p[1].setX(x+wh);
        p[1].setY(y-wh);
        p[2].setX(x+wh);
        p[2].setY(y+wh);
        p[3].setX(x-wh);
        p[3].setY(y+wh);
        p[4].setX(x-wh);
        p[4].setY(y-wh);
        npoints=5;
    break;
    case SYM_DIAMOND://diamond
        p[0].setX(x);
        p[0].setY(y-wh);
        p[1].setX(x+wh);
        p[1].setY(y);
        p[2].setX(x);
        p[2].setY(y+wh);
        p[3].setX(x-wh);
        p[3].setY(y);
        p[4].setX(x);
        p[4].setY(y-wh);
        npoints=5;
        break;
    case SYM_TRIANG1://triangle up
        p[0].setX(x);
        p[0].setY(y-wh);
        p[1].setX(x+wh);
        p[1].setY(y+w/4);
        p[2].setX(x-wh);
        p[2].setY(y+w/4);
        p[3].setX(x);
        p[3].setY(y-wh);
        npoints=4;
        break;
    case SYM_TRIANG2://triangle left
        p[0].setX(x-wh);
        p[0].setY(y);
        p[1].setX(x+w/4);
        p[1].setY(y-wh);
        p[2].setX(x+w/4);
        p[2].setY(y+wh);
        p[3].setX(x-wh);
        p[3].setY(y);
        npoints=4;
        break;
    case SYM_TRIANG3://triangle down
        p[0].setX(x);
        p[0].setY(y+wh);
        p[1].setX(x+wh);
        p[1].setY(y-w/4);
        p[2].setX(x-wh);
        p[2].setY(y-w/4);
        p[3].setX(x);
        p[3].setY(y+wh);
        npoints=4;
        break;
    case SYM_TRIANG4://triangle right
        p[0].setX(x+wh);
        p[0].setY(y);
        p[1].setX(x-w/4);
        p[1].setY(y-wh);
        p[2].setX(x-w/4);
        p[2].setY(y+wh);
        p[3].setX(x+wh);
        p[3].setY(y);
        npoints=4;
        break;
    }
paint1.drawPolygon(p,npoints);
}

}

#define LIST_ICON_WIDTH 26//24
#define LIST_ICON_HEIGHT 14
#define LIST_ICON_Y_MID 7
#define LIST_ICON_X_MID1 7
#define LIST_ICON_X_MID2 14//12
#define LIST_ICON_X_SHIFT 12//10
#define LIST_ICON_SYMBOL_SIZE 5

QIcon createIconForSetAppearance(int gno,int sno)
{
QPixmap map1(LIST_ICON_WIDTH,LIST_ICON_HEIGHT);
QPainter paint1(&map1);
QPen tmp_pen;
bool only_one=false;
int has_x_errorbars,has_y_errorbars;
paint1.setPen(Qt::white);
paint1.setBrush(Qt::white);
paint1.drawRect(0,0,LIST_ICON_WIDTH+1,LIST_ICON_HEIGHT+1);
if (is_valid_setno(gno,sno))
{
has_x_errorbars=has_errorbar(gno, sno, 0);
has_y_errorbars=has_errorbar(gno, sno, 1);
/// draw a simple line
    if (g[gno].p[sno].lines!=0 && g[gno].p[sno].linet!=0)
    {
        if (g[gno].p[sno].sym==0)
        tmp_pen=createPen(g[gno].p[sno].lines,g[gno].p[sno].linew,g[gno].p[sno].linepen.color);
        else
        tmp_pen=createPen(g[gno].p[sno].lines,1.0,g[gno].p[sno].linepen.color);
paint1.setPen(tmp_pen);
paint1.setBackground(Qt::transparent);
        if (g[gno].p[sno].sym==0 && has_x_errorbars==FALSE && has_y_errorbars==FALSE)//no symbol and no error bars --> draw the line completely from one side to the other
        paint1.drawLine(0,LIST_ICON_Y_MID,LIST_ICON_WIDTH+1,LIST_ICON_Y_MID);
        else
        paint1.drawLine(LIST_ICON_X_MID1,LIST_ICON_Y_MID,LIST_ICON_WIDTH-LIST_ICON_X_MID1,LIST_ICON_Y_MID);
    only_one=false;
    }
    else
    {
    only_one=true;
    }
/// draw error-bars
if (g[gno].p[sno].errbar.active && has_y_errorbars)/// Y-error-bars
{
tmp_pen=createPen(g[gno].p[sno].errbar.lines,1.0,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(4,1,4+LIST_ICON_SYMBOL_SIZE,1);//upper bar (left)
        paint1.drawLine(LIST_ICON_WIDTH-4,1,LIST_ICON_WIDTH-4-LIST_ICON_SYMBOL_SIZE,1);//upper bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2-0.5*LIST_ICON_SYMBOL_SIZE,1,LIST_ICON_X_MID2+0.5*LIST_ICON_SYMBOL_SIZE,1);//upper bar
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(4,LIST_ICON_HEIGHT-2,4+LIST_ICON_SYMBOL_SIZE,LIST_ICON_HEIGHT-2);//lower bar (left)
        paint1.drawLine(LIST_ICON_WIDTH-4,LIST_ICON_HEIGHT-2,LIST_ICON_WIDTH-4-LIST_ICON_SYMBOL_SIZE,LIST_ICON_HEIGHT-2);//lower bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_HEIGHT-2,LIST_ICON_X_MID2+0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_HEIGHT-2);//lower bar
        }
    }
tmp_pen=createPen(g[gno].p[sno].errbar.riser_lines,1.0,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1,LIST_ICON_Y_MID,LIST_ICON_X_MID1,1);//left upper riser
        paint1.drawLine(LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,LIST_ICON_Y_MID,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,1);//right upper riser
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2,LIST_ICON_Y_MID,LIST_ICON_X_MID2,1);//upper bar
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1,LIST_ICON_Y_MID,LIST_ICON_X_MID1,LIST_ICON_HEIGHT-2);//left lower riser
        paint1.drawLine(LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,LIST_ICON_Y_MID,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,LIST_ICON_HEIGHT-2);//right lower riser
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2,LIST_ICON_Y_MID,LIST_ICON_X_MID2,LIST_ICON_HEIGHT-2);//lower riser
        }
    }
}
if (g[gno].p[sno].errbar.active && has_x_errorbars)/// X-error-bars
{
tmp_pen=createPen(g[gno].p[sno].errbar.lines,1.0,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_X_MID1+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID+0.5*LIST_ICON_SYMBOL_SIZE);//right bar (left)
        paint1.drawLine(LIST_ICON_X_MID1+LIST_ICON_X_SHIFT+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID+0.5*LIST_ICON_SYMBOL_SIZE);//right bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_X_MID2+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID+0.5*LIST_ICON_SYMBOL_SIZE);//right bar
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_X_MID1-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID+0.5*LIST_ICON_SYMBOL_SIZE);//left bar (left)
        paint1.drawLine(LIST_ICON_X_MID1+LIST_ICON_X_SHIFT-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID+0.5*LIST_ICON_SYMBOL_SIZE);//left bar (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID-0.5*LIST_ICON_SYMBOL_SIZE,LIST_ICON_X_MID2-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID+0.5*LIST_ICON_SYMBOL_SIZE);//left bar
        }
    }
tmp_pen=createPen(g[gno].p[sno].errbar.riser_lines,1.0,g[gno].p[sno].errbar.pen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_NORMAL || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1,LIST_ICON_Y_MID,LIST_ICON_X_MID1+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID);//right riser (left)
        paint1.drawLine(LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,LIST_ICON_Y_MID,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID);//right riser (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2,LIST_ICON_Y_MID,LIST_ICON_X_MID2+2+LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID);//right riser
        }
    }
    if (g[gno].p[sno].errbar.ptype==PLACEMENT_OPPOSITE || g[gno].p[sno].errbar.ptype==PLACEMENT_BOTH)
    {
        if (only_one==false)
        {
        paint1.drawLine(LIST_ICON_X_MID1,LIST_ICON_Y_MID,LIST_ICON_X_MID1-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID);//left riser (left)
        paint1.drawLine(LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,LIST_ICON_Y_MID,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID);//left riser (right)
        }
        else
        {
        paint1.drawLine(LIST_ICON_X_MID2,LIST_ICON_Y_MID,LIST_ICON_X_MID2-2-LIST_ICON_SYMBOL_SIZE*0.5,LIST_ICON_Y_MID);//left riser
        }
    }
}
/// draw symbols
if (g[gno].p[sno].sym!=0)
{
tmp_pen=createPen(g[gno].p[sno].symlines,1.0,g[gno].p[sno].sympen.color);
paint1.setPen(tmp_pen);
    if (g[gno].p[sno].symfillpen.pattern==0)
    {
    //paint1.setBrush(Qt::white);
    paint1.setBrush(Qt::NoBrush);
    }
    else
    {
    paint1.setBrush(get_Color(g[gno].p[sno].symfillpen.color));
    }
        if (g[gno].p[sno].sym==SYM_CHAR)
        {
        QFont dFont=getFontFromDatabase(g[gno].p[sno].charfont);
        dFont.setPixelSize(LIST_ICON_SYMBOL_SIZE*1.8);
        paint1.setFont(dFont);
        }
        if (only_one==false)
        {
        drawSimpleSymbol(paint1,LIST_ICON_X_MID1,LIST_ICON_Y_MID,LIST_ICON_SYMBOL_SIZE,g[gno].p[sno].sym,g[gno].p[sno].symchar);
        drawSimpleSymbol(paint1,LIST_ICON_X_MID1+LIST_ICON_X_SHIFT,LIST_ICON_Y_MID,LIST_ICON_SYMBOL_SIZE,g[gno].p[sno].sym,g[gno].p[sno].symchar);
        }
        else
        {
        drawSimpleSymbol(paint1,LIST_ICON_X_MID2,LIST_ICON_Y_MID,LIST_ICON_SYMBOL_SIZE,g[gno].p[sno].sym,g[gno].p[sno].symchar);
        }
}
paint1.end();
}

return QIcon(map1);
}

uniList::uniList(int type,QWidget *parent):QListWidget(parent)
{
setIconSize(QSize(LIST_ICON_WIDTH,LIST_ICON_HEIGHT));
    minimum_display=false;
    prevent_from_autoupdate=false;
    datType=type;
    partner=NULL;
    show_all_sets_marker=false;
    all_entries_option_selected=false;
    //allowSnycCommand=false;

    //set the standard-behavior (can be changed especially for some widgets)
    if (type==GRAPHLIST)
    {
        setBehavior(true,false,false);
    }
    else if (type==SETCHOICE)
    {
        setBehavior(false,true,true);
    }
    else//SetList
    {
        setBehavior(false,true,false);
    }
    connect(this,SIGNAL(itemDoubleClicked(QListWidgetItem *)),SLOT(entryDoubleClicked(QListWidgetItem *)));
    connect(this,SIGNAL(itemSelectionChanged()),SLOT(new_selection()));

    popupMenu1=new SetPopup(this);
    popupMenu1->hide();
    popupMenu2=new GraphPopup(this);
    popupMenu2->hide();

    actCopyClipBoard=new QAction(tr("Copy"),this);
    actCopyClipBoard->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    actPasteClipBoard=new QAction(tr("Paste"),this);
    actPasteClipBoard->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_V));
    if (datType==SETLIST)
    {
        this->addAction(actCopyClipBoard);
        this->addAction(actPasteClipBoard);
        connect(actCopyClipBoard,SIGNAL(triggered()),SLOT(doCopyClipBoard()));
        connect(actPasteClipBoard,SIGNAL(triggered()),SLOT(doPasteClipBoard()));
    }

    number_of_entries=0;
    gr_no=0;
    entries=new int[2];
    text_entries=new QString[2];

    show_hidden=DefaultSetListShowHidden;
    show_data_less=DefaultSetListShowDataless;
    show_comments=DefaultSetListShowComments;
    show_legends=DefaultSetListShowLegends;
    show_icons=DefaultSetListShowIcons;

    if (type==GRAPHLIST)//register this List so that it may be updated later (live)
    {
        add_GraphSelector(this);
    }
    else if (type==SETCHOICE)
    {
        add_SetChoiceItem(this);
    }
    else
    {
        add_SetSelector(this);
    }
}

uniList::~uniList()
{
    if (entries)
    delete[] entries;
    if (text_entries)
    delete[] text_entries;
}

void uniList::update_number_of_entries(void)
{
    char dummy[2048];
    char hidden_char;
    int cols;
    int index=0;
    QString itemtext;
    QIcon tmp_icon;
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    clear();
    if (datType==GRAPHLIST)
    {
        delete[] entries;
        entries=new int[number_of_graphs()>1?number_of_graphs()+1:2+1];
        if (show_all_sets_marker==true && number_of_graphs()>1)
        {
            if (minimum_display==false)
                itemtext=tr("All graphs");
            else
                itemtext=tr("All");
            new QListWidgetItem(itemtext, this);
            entries[index]=-1;
            index++;
        }
        for (int i=0;i<number_of_graphs();i++)
        {
            if (g[i].hidden!=0 && show_hidden==false) continue;//don't show hidden graphs
            hidden_char=is_graph_hidden(i)?'-':'+';
            ///sprintf(dummy,"(%c) G%d (%d sets)",hidden_char,i,number_of_sets(i));
            if (minimum_display==false)
                sprintf(dummy,"(%c) G%d (%d sets)",hidden_char,i,number_of_real_sets(i));
            else
                sprintf(dummy,"(%c) G%d",hidden_char,i);
            new QListWidgetItem(QString(dummy), this);
            entries[index]=i;
            index++;
        }
    }
    else if (datType==TEXTLIST)
    {
        index=number_of_entries;//This should by all that is needed.
    }
    else//SETLIST or SETCHOICE
    {
        if (gr_no<0 || gr_no>=number_of_graphs()) return;
        delete[] entries;
        entries=new int[g[gr_no].maxplot>1?g[gr_no].maxplot+1:2+1];
        //we count the number of entries
        cols=0;
        for (int i=0;i<g[gr_no].maxplot;i++)
        {
        if (!((show_data_less == true || is_set_active(gr_no, i) == TRUE) && (show_hidden == true || is_set_hidden(gr_no, i) != TRUE ))) continue;
        cols++;
        }
        if (show_all_sets_marker==true && cols>1)//only show all-entries-marker if more htan one set is present
        {
            if (minimum_display==false)
                itemtext=tr("All sets");
            else
                itemtext=tr("All");
            new QListWidgetItem(itemtext, this);
            entries[index]=-1;
            index++;
        }
        for (int i=0;i<g[gr_no].maxplot;i++)
        {
            if (!((show_data_less == true || is_set_active(gr_no, i) == TRUE) && (show_hidden == true || is_set_hidden(gr_no, i) != TRUE ))) continue;
            ///if ((g[gr_no].p[i].hidden!=0 && show_hidden==false) || g[gr_no].p[i].data.len<=0) continue;//don't show hidden sets
            hidden_char=g[gr_no].p[i].hidden==0?'+':'-';
            cols=settype_cols(g[gr_no].p[i].type);
            if (datType==SETLIST)
            {
                if (minimum_display==false)
                    sprintf(dummy,"(%c) G%d.S%d[%d][%d]",hidden_char,gr_no,i,cols,g[gr_no].p[i].data.len);
                else
                    sprintf(dummy,"(%c) S%d",hidden_char,i);
                itemtext=QString(dummy);
                if (show_legends)
                {
                    /*strcat(dummy," \"");
                    strcat(dummy,QString::fromUtf8(g[gr_no].p[i].orig_lstr).toLocal8Bit().constData());
                    strcat(dummy,"\"");*/
                    itemtext+=QString(" \"")+QString::fromUtf8(g[gr_no].p[i].orig_lstr)+QString("\"");
                }
                if (show_comments)
                {
                    /*if (show_legends) strcat(dummy,", ");
                    strcat(dummy," \"");
                    strcat(dummy,QString::fromUtf8(g[gr_no].p[i].orig_comments).toLocal8Bit().constData());
                    strcat(dummy,"\"");*/
                    if (show_legends) itemtext+=QString(", ");
                    itemtext+=QString(" \"")+QString::fromUtf8(g[gr_no].p[i].orig_comments)+QString("\"");
                }
            }
            else//setChoice
            {
                sprintf(dummy, "S%d", i);
                itemtext=QString(dummy);
                if (minimum_display==false)
                {
                itemtext+=QString(" (N=")+QString::number(getsetlength(gr_no, i))+QString(", ")+QString::fromUtf8(g[gr_no].p[i].orig_comments)+QString(")");
                //sprintf(dummy, "S%d (N=%d, %s)", i, getsetlength(gr_no, i), getcomment(gr_no, i));
                }
            }
            if (show_icons)
            {
            tmp_icon=createIconForSetAppearance(gr_no,i);
            new QListWidgetItem(tmp_icon, itemtext, this);
            }
            else
            {
            new QListWidgetItem(itemtext, this);
            }
                if (g[gr_no].p[i].hidden==0)
                this->item(index)->setForeground(Qt::black);
                else
                this->item(index)->setForeground(Qt::gray);
            entries[index]=i;
            index++;
        }
    }
    number_of_entries=index;
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::update_number_of_entries_preserve_selection(void)
{
    int ns,*sel=new int[2];
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    get_selection(&ns,&sel);

/*qDebug() << "selection:";
for (int j=0;j<ns;j++)
qDebug() << j << ": " << sel[j];
qDebug() << "number_of_entries=" << this->number_of_entries;*/

/*
if (0)//partner!=NULL)//this part did not work as desired --> I think this could be deleted
{
disconnect(partner,SIGNAL(itemSelectionChanged()),partner,SLOT(new_selection()));
    int nsp,*selp=new int[2];
    partner->get_selection(&nsp,&selp);
    cout << "selection:" << endl;
    for (int j=0;j<ns;j++)
    cout << j << ": " << sel[j] << endl;
    cout << "partner_selection:" << endl;
    for (int j=0;j<nsp;j++)
    cout << j << ": " << selp[j] << endl;
   if (datType==GRAPHLIST)//nothing to be done here, everything is done, when set is updated
   {//partner is a set-list
        cout << "t1" << endl;
        update_number_of_entries();
        cout << "t2" << endl;
        set_new_selection(ns,sel);
        cout << "t3" << endl;
        partner->update_number_of_entries();
        cout << "t4" << endl;
        partner->set_new_selection(nsp,selp);
        cout << "t5" << endl;
        //cout << "test Graphlist-update" << endl;//update_number_of_entries();
   }
   else
   {
   cout << "test Setlist-update -- doing nothing!" << endl;
   }
   delete[] selp;
connect(partner,SIGNAL(itemSelectionChanged()),partner,SLOT(new_selection()));
}
else
{
*/
    if (datType==TEXTLIST)
    {
        clear();
        for (int i=0;i<number_of_entries;i++)
        {
            new QListWidgetItem(text_entries[i], this);
        //cout << "update TEXTLIST: #" << text_entries[i].toLocal8Bit().constData() << "#" << endl;
        }
    }
    else
    {
        update_number_of_entries();
    }
    set_new_selection(ns,sel);
    new_selection();
    //}
    if (sel) delete[] sel;
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::set_graph_number(int gno,bool keep_selection)
{
    if (gno<0) gr_no=get_cg();
    else gr_no=gno;
int s_nr,*sel=new int[2];
    get_selection(&s_nr,&sel);
    update_number_of_entries();
    if (datType==GRAPHLIST)
        item(gno)->setSelected(TRUE);
    else if (keep_selection==true)
        set_new_selection(s_nr,sel);
    if (sel) delete[] sel;
}

void uniList::get_selection(int * number,int ** selection)
{
    if (count()<=0)
    {
    *number=0;
    return;
    }
    QModelIndexList list=selectedIndexes();
    *number=list.size();
    if (*number<=0) return;//return without deleting anything
        if (*selection!=NULL)
        {
        delete[] *selection;
        *selection=NULL;
        }
    if (entries[list.at(0).row()]<0)//all_entries
    {
        all_entries_option_selected=true;
        *number=number_of_entries-1;
        *selection=new int[*number+2];
        for (int i=0;i<*number;i++)
            *(*selection+i)=entries[i+1];
    }
    else
    {
        all_entries_option_selected=false;
        *selection=new int[*number+2];
        for (int i=0;i<*number;i++)
            *(*selection+i)=entries[list.at(i).row()];
    }
    sort(*number,*selection);
}

void uniList::set_new_selection(int number,int * selection)
{
    disconnect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
    int scroll=-1;
    if (number<0) goto end_set_new_selection;
    clearSelection();
    if (number==0) goto end_set_new_selection;
    for (int i=0;i<number;i++)
    {
        //if (selection[i]>count()) continue;//this is useless because the count does not say what numbers are present!
        for (int j=0;j<count();j++)
        {
            if (selection[i]==entries[j])
            {
                item(j)->setSelected(TRUE);
                if (scroll==-1) scroll=j;
            }
        }
    }
    if (scroll!=-1)
        scrollToItem(item(scroll));
end_set_new_selection:
    connect(this,SIGNAL(itemSelectionChanged()),this,SLOT(new_selection()));
}

void uniList::new_selection(void)
{
QModelIndexList list=selectedIndexes();
    if (list.size()>0)
    {
        emit(new_selection(list.at(0).row()));
    }
    else
    {
        emit(new_selection(-1));
    }
    if (mainWin->highlightTimer->isActive() && this->hasFocus())
    {
    mainWin->stop_highlighting();
    QKeyEvent ev_key(QEvent::KeyPress,Qt::Key_H,Qt::NoModifier);
    this->keyPressEvent(&ev_key);
    }
    else if (!mainWin->highlightTimer->isActive() && this->hasFocus() && auto_highlight==TRUE)
    {
    //qDebug() << "Now we should start the highlighting";
    activateHighlighting();
    }
}

void uniList::mousePressEvent(QMouseEvent * e)
{
    Qt::KeyboardModifiers modif=e->modifiers();
    QCursor curs=mainWin->cursor();
    QCursor curs2(Qt::WhatsThisCursor);
    QCursor curs3=cursor();
    QCursor curs4=parentWidget()->cursor();
    if (curs.shape()==curs2.shape() || curs3.shape()==curs2.shape() || curs4.shape()==curs2.shape())
    {
        if (datType==SETLIST)
        {
            HelpCB("doc/UsersGuide.html#set-selector");
        }
        else if (datType==GRAPHLIST)
        {
            HelpCB("doc/UsersGuide.html#graph-selector");
        }
        else
        {
            HelpCB("doc/UsersGuide.html#list-selector");
        }
        unsetCursor();
        return;
    }
    QPoint p=e->pos();
    QModelIndexList list=selectedIndexes();
    int number_of_selected_entries=list.size();
    QListWidgetItem * clickedItem=itemAt(p);
    int selected_no=row(clickedItem);

    bool already_selected=false;
    if (clickedItem!=0)
        already_selected=item(selected_no)->isSelected();

    if (e->button() & Qt::RightButton)
    {
        if (datType==SETLIST)
        {
            if (popupMenu1==NULL) return;
            popupMenu1->selected_graph=gr_no;
            get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
            sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
            popupMenu1->update_menu_content();
            popupMenu1->exec(QCursor::pos());
            //popupMenu1->exec(e->globalPos());
        }
        else if (datType==GRAPHLIST)
        {
            if (popupMenu2==NULL) return;
            get_selection(&popupMenu2->number_of_selected_graphs,&popupMenu2->selected_graphs);
            sort(popupMenu2->number_of_selected_graphs,popupMenu2->selected_graphs);
            popupMenu2->selected_no=selected_no;
            popupMenu2->update_menu_content();
            popupMenu2->exec(QCursor::pos());
            QListWidget::mousePressEvent(e);
        }
        e->accept();
    }
    else//Left Button clicked
    {
        if (clickedItem==0)
        {
            return;//nothing to do if someone clicked outside the entries
        }
        if (number_of_selected_entries>1 && ((int)(modif&Qt::ControlModifier))==0)
        {
            clearSelection();
            item(selected_no)->setSelected(true);
        }
        else if (deselect_possible==true && already_selected==true)
        {
            item(selected_no)->setSelected(false);
        }
        else
        {
            QListWidget::mousePressEvent(e);
        }
    }
}

void uniList::mouseReleaseEvent(QMouseEvent * e )
{
    QPoint p=e->pos();
    //QModelIndexList list=selectedIndexes();
    //int number_of_selected_entries=list.size();
    QListWidgetItem * clickedItem=itemAt(p);
    //int selected_no=row(clickedItem);
    if (clickedItem!=0)
        QListWidget::mouseReleaseEvent(e);
}

void uniList::activateHighlighting(void)
{
    if (mainWin->highlightTimer->isActive()) return;
//qDebug() << "start highlighting";
int nr_sel,*sel=new int[2],*g_sel=new int[2];
get_selection(&nr_sel,&sel);
    if (datType==GRAPHLIST)
    {
    prepare_highlight_segments_with_graphs(nr_sel,sel);
    mainWin->start_highlighting();
    }
    else if (datType==SETLIST)
    {
    delete[] g_sel;
    g_sel=new int[nr_sel];
    for (int j=0;j<nr_sel;j++) g_sel[j]=gr_no;
    prepare_highlight_segments_with_sets(nr_sel,g_sel,sel);
    mainWin->start_highlighting();
    }
if (sel!=NULL) delete[] sel;
if (g_sel!=NULL) delete[] g_sel;
}

void uniList::keyPressEvent(QKeyEvent * e)
{
int k=e->key();
if (k==Qt::Key_H)
{
//qDebug() << "H-key pressed";
    if (mainWin->highlightTimer->isActive())
    mainWin->stop_highlighting();
    else
    activateHighlighting();
}
/*else if (k==Qt::Key_Escape)
{
mainWin->stop_highlighting();
}*/
else if (k==Qt::Key_Delete)
{
   if (datType==GRAPHLIST)
   {
   popupMenu2->doKill();
   }
   else if (datType==SETLIST)
   {
   popupMenu1->doKill();
   }
}
else if (k==Qt::Key_A && (e->modifiers()&Qt::ControlModifier))
{
    if (datType==SETLIST)
    {
    popupMenu1->doSelectAll();
    }
}
else if (k==Qt::Key_Up || k==Qt::Key_Down)
{
e->accept();
int delta=(k==Qt::Key_Down?1:-1),scroll=-1;
QModelIndexList list=selectedIndexes();
if (list.length()<1) return;//nothing to do, because nothing is selected
int nr_of_sel=list.length(),*sel_ids=new int[2+list.length()];
blockSignals(true);
clearSelection();
//get_selection(&nr_of_sel,&sel_ids);
for (int i=0;i<nr_of_sel;i++)
{
sel_ids[i]=list.at(i).row()+delta;
}
//set_new_selection(nr_of_sel,sel_ids);
for (int i=0;i<nr_of_sel;i++)
{
    if (sel_ids[i]>=0 && sel_ids[i]<count())
    {
    item(sel_ids[i])->setSelected(TRUE);
    if (scroll==-1) scroll=sel_ids[i];
    }
}
if (scroll<0 && count()>0)//no selection any more but there is still something to select
{
    if (k==Qt::Key_Down) scroll=count()-1;
    else if (k==Qt::Key_Up) scroll=0;
item(scroll)->setSelected(TRUE);
}
if (scroll>=0 && scroll<count()) scrollToItem(item(scroll));
if (sel_ids) delete[] sel_ids;
blockSignals(false);
if (scroll>=0 && scroll<count()) emit(new_selection(scroll));
}
else e->ignore();
//QListWidget::keyPressEvent(e);
}

void uniList::keyReleaseEvent(QKeyEvent * e)
{
int k=e->key();
    if (datType==GRAPHLIST || datType==SETLIST)
    {
        Qt::KeyboardModifiers mod=e->modifiers();
        if (mod==Qt::ControlModifier)
        {
            if (k==Qt::Key_I)
            {
                popupMenu1->selected_graph=gr_no;
                get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
                sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
                popupMenu1->doInvertSelection();
            }
            else if(k==Qt::Key_U)
            {
                clearSelection();
            }
            else e->ignore();
        }
        else e->ignore();
    }
}

void uniList::focusOutEvent(QFocusEvent *event)
{
(void)event;
//qDebug() << "H-Key released";
mainWin->stop_highlighting();
}

void uniList::entryDoubleClicked(QListWidgetItem * c_item)
{
    if (datType==SETLIST)
    {
        int cl_index=-1;
        for (int i=0;i<number_of_entries;i++)
        {
            if (item(i)==c_item)
            {
                cl_index=i;
                break;
            }
        }
        if (cl_index>=0)
            showSetInSpreadSheet(gr_no,entries[cl_index]);
    }
    else if (datType==GRAPHLIST)
    {
        int nr_sel,*sel=new int[2];
        get_selection(&nr_sel,&sel);
        int cg = get_cg();
        if (sel[0] != cg) {
            switch_current_graph(sel[0]);
            mainWin->mainArea->completeRedraw();
        }
        delete[] sel;
    }
}

void uniList::clear_text_items(void)
{
delete[] text_entries;
delete[] entries;
number_of_entries=0;
clear();
}

void uniList::add_Item(QString item)
{
    QString * str=new QString[number_of_entries+3];
    int * tentr=new int[number_of_entries+2];
    for (int i=0;i<number_of_entries;i++)
    {
        str[i]=text_entries[i];
        tentr[i]=entries[i];
    }
    str[number_of_entries]=item;
    tentr[number_of_entries]=number_of_entries;
    delete[] text_entries;
    delete[] entries;
    text_entries=str;
    entries=tentr;
    number_of_entries++;
    update_number_of_entries_preserve_selection();
}

void uniList::get_selection(int * number,QString ** selection)
{
    QModelIndexList list=selectedIndexes();
    *number=list.size();
    if (*number<=0) return;
    delete[] *selection;
    *selection=new QString[*number];
    for (int i=0;i<*number;i++)
        *(*selection+i)=text_entries[list.at(i).row()];
    //sort(*number,*selection);
}

void uniList::setBehavior(bool deselect,bool multi,bool all_sets)
{
    deselect_possible=deselect;
    show_all_sets_marker=all_sets;
    if (multi==true)
    {
        setSelectionMode(QAbstractItemView::ExtendedSelection);
    }
    else
    {
        setSelectionMode(QAbstractItemView::SingleSelection);
    }
}

void uniList::doCopyClipBoard(void)
{
    if (datType!=SETLIST || selectedIndexes().size()<=0) return;//nothing suitable selected
    popupMenu1->selected_graph=gr_no;
    get_selection(&popupMenu1->number_of_selected_sets,&popupMenu1->selected_sets);
    sort(popupMenu1->number_of_selected_sets,popupMenu1->selected_sets);
    popupMenu1->update_menu_content();
    popupMenu1->doCopyClipBoard();
}

void uniList::doPasteClipBoard(void)
{
    if (QApplication::clipboard()->mimeData()->hasUrls() || QApplication::clipboard()->mimeData()->hasText())
    {
        GeneralPaste(QApplication::clipboard()->mimeData());
    }
}

/*void uniList::doSyncNow(void)
{
    emit(syncSelection());
}*/

TreePopup::TreePopup(treeView *parent):QMenu(parent)
{
    par2=NULL;
    par=parent;
    selected_type='0';
    gno=sno=-1;
    setTearOffEnabled(FALSE);
    CreateActions();
    //Create Menu itself
    addAction(actShow);
    addAction(actHide);
    addAction(actKill);
    addAction(actDuplicate);
}

TreePopup::~TreePopup()
{

}

void TreePopup::CreateActions(void)
{
    actShow=new QAction(tr("Show"),this);
    connect(actShow,SIGNAL(triggered()),SLOT(doShow()));
    actHide=new QAction(tr("Hide"),this);
    connect(actHide,SIGNAL(triggered()),SLOT(doHide()));
    actKill=new QAction(tr("Kill"),this);
    connect(actKill,SIGNAL(triggered()),SLOT(doKill()));
    actDuplicate=new QAction(tr("Duplicate"),this);
    connect(actDuplicate,SIGNAL(triggered()),SLOT(doDuplicate()));
}

void TreePopup::doNothing(void)
{}

void TreePopup::doShow(void)
{
    doAction(1);
}

void TreePopup::doHide(void)
{
    doAction(2);
}

void TreePopup::doKill(void)
{
    doAction(3);
}

void TreePopup::doDuplicate(void)
{
    doAction(4);
}

void TreePopup::doAction(int type)//type is the kind of action
{
    int obj_type;
    bool d1;
    tickmarks * t2;
    char descr_axis[512];
    int undoObjIds[2];
    int undoObjIds2[2];
    legend l2;
    ListOfChanges.clear();
    ListOfOldStates.clear();
    switch ((int)selected_type)
    {
    default:
    case (int)'0'://nothing
        ;
        break;
    case (int)'N'://String-Object
    case (int)'I'://Line-Object
    case (int)'B'://Box-Object
    case (int)'P'://Ellipse-Object
        if (selected_type=='N')
            obj_type=OBJECT_STRING;
        else if (selected_type=='I')
            obj_type=OBJECT_LINE;
        else if (selected_type=='B')
            obj_type=OBJECT_BOX;
        else//'P'
            obj_type=OBJECT_ELLIPSE;
        undoObjIds[0]=gno;
        if (type==1)//show
        {
            SaveObjectData(undoObjIds[0],obj_type);
            set_object_active(obj_type,undoObjIds[0],TRUE);
            ObjectDataModified(undoObjIds[0],obj_type);
        }
        else if (type==2)//hide
        {
            SaveObjectData(undoObjIds[0],obj_type);
            set_object_active(obj_type,undoObjIds[0],FALSE);
            ObjectDataModified(undoObjIds[0],obj_type);
        }
        else if (type==3)//kill
        {
            ObjectsDeleted(1,undoObjIds,obj_type);
            kill_object(obj_type,undoObjIds[0]);
        }
        else if (type==4)//duplicate
        {
            undoObjIds[1]=duplicate_object(obj_type,undoObjIds[0]);
            ObjectsCreated(1,undoObjIds+1,obj_type);
        }
        break;
    case (int)'E'://Legend
        if (type==1)//show
            d1=true;
        else//hide
            d1=false;
        get_graph_legend(gno,&l2);
        undoObjIds[0]=gno;
        SaveGraphStatesPrevious(1,undoObjIds,UNDO_APPEARANCE);
        sprintf(dummy,"with g%d",gno);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        sprintf(dummy,"    legend %s",d1?"on":"off");
        ListOfChanges << QString(dummy);
        sprintf(dummy,"    legend %s",l2.active?"on":"off");
        ListOfOldStates << QString(dummy);
        l2.active=(int)d1;
        set_graph_legend(gno, &l2);
        GraphsModified(1,undoObjIds,UNDO_APPEARANCE);
        addAditionalDescriptionToLastNode(-1,tr("Graph legend visibility changed"),QString(),-1);
        break;
    case (int)'S'://Set
        undoObjIds[0]=gno;
        undoObjIds2[0]=sno;
        if (type==1)//show
        {
            set_set_hidden(gno, sno, FALSE);
            ShowHideSets(1,undoObjIds,undoObjIds2);
        }
        else if (type==2)//hide
        {
            set_set_hidden(gno, sno, TRUE);
            ShowHideSets(1,undoObjIds,undoObjIds2);
        }
        else if (type==3)//kill
        {
            SetsDeleted(1,undoObjIds,undoObjIds2,UNDO_COMPLETE);
            killset(gno,sno);
        }
        else if (type==4)//duplicate
        {
            undoObjIds2[0]=nextset(gno);
            do_copyset(gno, sno, gno, undoObjIds2[0]);
            SetsCreated(1,undoObjIds,undoObjIds2,UNDO_COMPLETE);
        }
        break;
    case (int)'A'://Axis
        if (type==1)//show
            d1=true;
        else//hide
            d1=false;
        t2 = get_graph_tickmarks(gno,sno);
        switch (sno)
        {
        case 0:
            strcpy(descr_axis,"xaxis");
            break;
        case 1:
            strcpy(descr_axis,"yaxis");
            break;
        case 2:
            strcpy(descr_axis,"altxaxis");
            break;
        case 3:
            strcpy(descr_axis,"altyaxis");
            break;
        }
        sprintf(dummy,"with g%d",gno);
        ListOfChanges << QString(dummy);
        ListOfOldStates << QString(dummy);
        if (int(d1)!=t2->active)
        {
            sprintf(dummy,"    %s %s",descr_axis,d1?"on":"off");
            ListOfChanges << QString(dummy);
            sprintf(dummy,"    %s %s",descr_axis,t2->active?"on":"off");
            ListOfOldStates << QString(dummy);
        }
        t2->active=(int)d1;
        break;
    case (int)'R'://General Settings
    case (int)'L'://Label/Title
        ;//no popup here --> this is not used at all
        break;
    case (int)'G'://Graph
        undoObjIds[0]=gno;
        undoObjIds2[0]=sno;
        if (type==1)//show
        {
            sprintf(dummy,"g%d hidden false",gno);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden %s",gno,is_graph_hidden(gno)?"true":"false");
            ListOfOldStates << QString(dummy);
            set_graph_hidden(gno, FALSE);
            ShowHideGraphs(1,undoObjIds);
        }
        else if (type==2)//hide
        {
            sprintf(dummy,"g%d hidden true",gno);
            ListOfChanges << QString(dummy);
            sprintf(dummy,"g%d hidden %s",gno,is_graph_hidden(gno)?"true":"false");
            ListOfOldStates << QString(dummy);
            set_graph_hidden(gno, TRUE);
            ShowHideGraphs(1,undoObjIds);
        }
        else if (type==3)//kill
        {
            GraphsDeleted(1,undoObjIds,UNDO_COMPLETE);
            sprintf(dummy,"kill G%d",gno);
            ListOfChanges << QString(dummy);
            kill_graph(gno);
        }
        else if (type==4)//duplicate
        {
            new_set_no=-1;
            duplicate_graph(gno);
            //I don't know the command for duplicating graphs
            if (new_set_no!=-1)
            {
                undoObjIds[0]=new_set_no;
                GraphsCreated(1,undoObjIds,UNDO_COMPLETE);
            }
        }
        break;
    }
    ListOfChanges.clear();
    ListOfOldStates.clear();
    mainWin->mainArea->completeRedraw();
    QTreeWidgetItem * ite=par->findItem(selected_type,gno,sno);
    par->itemClickedAt(ite,0);
}

void TreePopup::updateEntries(char ty,int g,int s)
{
    selected_type=ty;
    gno=g;
    sno=s;
    possible=true;
    actShow->setVisible(true);
    actHide->setVisible(true);
    actKill->setVisible(true);
    actDuplicate->setVisible(true);
    switch (int(ty))
    {
    case (int)'0':
    case (int)'R'://General
    case (int)'L'://Label/Title
        possible=false;
        break;
    case (int)'A'://Axis
    case (int)'E'://Legend
        //only show and hide
        actKill->setVisible(false);
        actDuplicate->setVisible(false);
        break;
    default:
    case (int)'N'://String-Object
    case (int)'I'://Line-Object
    case (int)'B'://Box-Object
    case (int)'P'://Ellipse-Object
    case (int)'G'://Graph
    case (int)'S'://Set
        //everything possible
        break;
    }
}

treeView::treeView(QWidget * parent):QTreeWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setColumnCount(1);
    setHeaderLabel(tr("Project..."));
    popup=new TreePopup(this);
    popup->hide();

    connect(this,SIGNAL(itemClicked(QTreeWidgetItem*,int)),SLOT(itemClickedAt(QTreeWidgetItem*,int)));
}

void treeView::NameProject(void)
{

}

void treeView::AddMajorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item)
{
(void)parent;
(void)item;
}

void treeView::AddMinorChild(QTreeWidgetItem * parent,QTreeWidgetItem * item)
{
(void)parent;
(void)item;
}

void treeView::ClearAll(void)
{
    this->clear();
}

void treeView::RecreateCompleteTree(void)
{
    ClearAll();
    char dummy[2048];
    QString dstr;
    QTreeWidgetItem * twi;
    QTreeWidgetItem * twi2;
    //general
    twi=new QTreeWidgetItem(this);
    twi->setText(0,tr("General Settings"));
    twi->setData(0,TREE_ROLE_GRAPH,QVariant(-1));
    twi->setData(0,TREE_ROLE_SET,QVariant(-1));
    twi->setData(0,TREE_ROLE_TYPE,QVariant('R'));
    twi->setIcon(0,QIcon(*ActiveIcon));
    addTopLevelItem(twi);
    //graphs
    for (int i=0;i<number_of_graphs();i++)
    {
        sprintf(dummy,"Graph G%d, type=%s",i,graph_types(g[i].type));
        twi=new QTreeWidgetItem(this);
        twi->setText(0,QString(dummy));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('G'));
        if (g[i].hidden==true)
            twi->setIcon(0,QIcon(*HiddenIcon));
        else
            twi->setIcon(0,QIcon(*ActiveIcon));
        addTopLevelItem(twi);
        //labels
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Title"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(0));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('L'));
        twi2->setIcon(0,QIcon(*ActiveIcon));
        twi->addChild(twi2);
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Subtitle"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(1));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('L'));
        twi2->setIcon(0,QIcon(*ActiveIcon));
        twi->addChild(twi2);
        //axis
        for (int j=0;j<MAXAXES;j++)
        {
            if (g[i].t[j] == NULL) continue;
            twi2=new QTreeWidgetItem(twi);
            switch (j)
            {
            case 0:
                dstr=tr("X axis");
                break;
            case 1:
                dstr=tr("Y axis");
                break;
            case 2:
                dstr=tr("Alt X axis");
                break;
            case 3:
                dstr=tr("Alt Y axis");
                break;
            }
            twi2->setText(0,dstr);
            twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
            twi2->setData(0,TREE_ROLE_SET,QVariant(j));
            twi2->setData(0,TREE_ROLE_TYPE,QVariant('A'));
            if (g[i].t[j]->active==false)
                twi2->setIcon(0,QIcon(*HiddenIcon));
            else
                twi2->setIcon(0,QIcon(*ActiveIcon));
            twi->addChild(twi2);
        }
        //all sets
        for (int j=0;j<number_of_sets(i);j++)
        {
            if (is_set_active(i, j) == FALSE) continue;
            sprintf(dummy,"Set S%d, type=%s",j,set_types(g[i].p[j].type));
            twi2=new QTreeWidgetItem(twi);
            twi2->setText(0,QString(dummy));
            twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
            twi2->setData(0,TREE_ROLE_SET,QVariant(j));
            twi2->setData(0,TREE_ROLE_TYPE,QVariant('S'));
            if (g[i].p[j].hidden==true)
                twi2->setIcon(0,QIcon(*HiddenIcon));
            else
                twi2->setIcon(0,QIcon(*ActiveIcon));
            twi->addChild(twi2);
        }
        //legend
        twi2=new QTreeWidgetItem(twi);
        twi2->setText(0,tr("Legend"));
        twi2->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi2->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi2->setData(0,TREE_ROLE_TYPE,QVariant('E'));
        if (g[i].l.active!=true)
            twi2->setIcon(0,QIcon(*HiddenIcon));
        else
            twi2->setIcon(0,QIcon(*ActiveIcon));
        twi->addChild(twi2);
    }//end graphs
    //strings
    for (int i = 0; i < maxstr; i++)
    {
        if (!isactive_string(i)) continue;
        twi=new QTreeWidgetItem(this);
        sprintf(dummy," \"%s\"",pstr[i].alt_plotstring);
        twi->setText(0,tr("String-Object")+QString(dummy));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('N'));
        if (pstr[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    //lines
    for (int i = 0; i < maxlines; i++)
    {
        if (!isactive_line(i)) continue;
        twi=new QTreeWidgetItem(this);
        twi->setText(0,tr("Line-Object"));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('I'));
        if (lines[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    //boxes
    for (int i = 0; i < maxboxes; i++)
    {
        if (!isactive_box(i)) continue;
        twi=new QTreeWidgetItem(this);
        twi->setText(0,tr("Box-Object"));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('B'));
        if (boxes[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    //ellipses
    for (int i = 0; i < maxellipses; i++)
    {
        if (!isactive_ellipse(i)) continue;
        twi=new QTreeWidgetItem(this);
        twi->setText(0,tr("Ellipse-Object"));
        twi->setData(0,TREE_ROLE_GRAPH,QVariant(i));
        twi->setData(0,TREE_ROLE_SET,QVariant(-1));
        twi->setData(0,TREE_ROLE_TYPE,QVariant('P'));
        if (ellip[i].active==TRUE)
            twi->setIcon(0,QIcon(*ActiveIcon));
        else
            twi->setIcon(0,QIcon(*HiddenIcon));
        addTopLevelItem(twi);
    }
    expandAll();
}

void treeView::getItemData(QTreeWidgetItem * item,char & type,int & gno,int & sno)
{
    type=item->data(0,TREE_ROLE_TYPE).toChar().toLatin1();
    gno=item->data(0,TREE_ROLE_GRAPH).toInt();
    sno=item->data(0,TREE_ROLE_SET).toInt();
}

QTreeWidgetItem * treeView::findItem(char type,int gno,int sno)
{
    char t;
    int g,s;
    QTreeWidgetItem * item=NULL;
    QTreeWidgetItem * item2=NULL;
    for (int i=0;i<topLevelItemCount();i++)
    {
        item=topLevelItem(i);
        getItemData(item,t,g,s);
        if (t==type && g==gno && s==sno) goto end_select_item;
        for (int j=0;j<item->childCount();j++)
        {
            item2=item->child(j);
            getItemData(item2,t,g,s);
            if (t==type && g==gno && s==sno)
            {
                item=item2;
                goto end_select_item;
            }
        }
    }
end_select_item:
    return item;
}

void treeView::itemClickedAt(QTreeWidgetItem * item,int col)
{
    char type;
    int gno;
    int sno;
    (void)col;
    getItemData(item,type,gno,sno);
    //cout << "type=" << type << " col=" << col << " G[" << gno << "].S[" << sno << "]" << endl;
    emit(newItemClicked(type,gno,sno));
}

void treeView::mousePressEvent(QMouseEvent * e)
{
    char type;
    int gno;
    int sno;
    QPoint p=e->pos();
    QTreeWidgetItem * curItem=itemAt(p);
    if (curItem==NULL) return;
    clearSelection();
    curItem->setSelected(true);
    if (e->button() & Qt::RightButton)
    {
        getItemData(curItem,type,gno,sno);
        //cout << type << " " << gno << " " << sno;
        popup->updateEntries(type,gno,sno);
        if (popup->possible==true)
            popup->exec(QCursor::pos());
    }
    else
    {
        itemClickedAt(curItem,0);
    }
}

void treeView::selectItem(char type,int gno,int sno)
{
    QTreeWidgetItem * item=findItem(type,gno,sno);
    if (item!=NULL)
    {
        clearSelection();
        item->setSelected(true);
    }
}

InLineEditor::InLineEditor(QWidget * parent):QLineEdit(parent)
{
    old_active_value=0;
    old_text=NULL;
edit_type=-1;//0=text;1=title;2=axis-label;3=legend
id1=id2=-1;
//edit_type=0 --> id1=graph_nr, id2=text-id
//edit_type=1 --> id1=graph_nr, id2=(0=title,1=sub_title)
//edit_type=2 --> id1=graph_nr, id2=axis-nr(x,y,alt-x,alt-y)
//edit_type=3 --> id1=graph_nr, id2=set_nr
bounding_box.setRect(1,1,5,5);//just to set something
this->setFrame(false);
this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
connect(this,SIGNAL(returnPressed()),this,SLOT(finish_editor()));
//QPalette pal;
pal=this->palette();
pal.setColor(QPalette::Base,Qt::transparent);
pal.setColor(QPalette::AlternateBase,Qt::transparent);
pal.setColor(QPalette::Window,Qt::transparent);
pal.setColor(QPalette::Button,Qt::transparent);
pal.setColor(QPalette::Text,Qt::black);
setPalette(pal);
hide();
}

void InLineEditor::init_editor(int type,int id_1,int id_2,int id_3,int id_4)
{
edit_type=type;//0=text;1=title;2=axis-label;3=legend;4=custom-tick
    id1=id_1;
    id2=id_2;
    id3=id_3;
    id4=id_4;
view bb1;
QFont fo1;
pal=this->palette();
//QColor spec_red(255,0,0,100);/// for testing
pal.setColor(QPalette::Base,Qt::transparent);
//pal.setColor(QPalette::Base,spec_red);/// for testing
Page_geometry cur_pg=get_page_geometry();
int rot;
double smaller=cur_pg.height<cur_pg.width?cur_pg.height:cur_pg.width;
this->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
int delta=0;
if (edit_type==0)/// Text-Object
{
this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    delta=0;
    if ( !is_valid_string(id_2) )
    {
    edit_type=-1;
    id1=id2=-1;
    }
    else
    {
    get_object_bb(OBJECT_STRING,id_2,&bb1);
    bounding_box=QRect(xconvxlib(pstr[id_2].x),win_h-xconvxlib(pstr[id_2].y),win_w,yconvxlib(bb1.yv1)-yconvxlib(bb1.yv2));
//qDebug() << "Bounding_Box (viewport): (" << bb1.xv1 << "|" << bb1.yv1 << ")-(" << bb1.xv2 << "|" << bb1.yv2 << ")";
    pal.setColor(QPalette::Text,get_Color(pstr[id_2].color));
    fo1=getFontFromDatabase(pstr[id_2].font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*pstr[id_2].charsize*GeneralPageZoomFactor*universal_font_size_factor);
/*qDebug() << "editor_font_size=" << fo1.pixelSize();
qDebug() << "Grace_font_size=" << pstr[id_2].charsize;
qDebug() << "global_factor=" << GeneralPageZoomFactor;
qDebug() << "global_font_factor=" << universal_font_size_factor;*/
    this->setText(QString::fromUtf8(pstr[id_2].alt_plotstring));
//qDebug() << "Bounding_Box (absolute): (" << bounding_box.x() << "|" << bounding_box.y() << "): w=" << bounding_box.width() << " h=" << bounding_box.height();
    }
}
else if (edit_type==1)/// Title
{
get_graph_viewport(id1,&bb1);
    if (id2==0)//title
    {
    bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv2+0.1),xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1),0.05*win_h);
    pal.setColor(QPalette::Text,get_Color(g[id1].labs.title.color));
    fo1=getFontFromDatabase(g[id1].labs.title.font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].labs.title.charsize*GeneralPageZoomFactor*universal_font_size_factor);
    this->setText(QString::fromUtf8(g[id1].labs.title.alt_plotstring));
    }
    else //subtitle
    {
    bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv2+0.05),xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1),0.05*win_h);
    pal.setColor(QPalette::Text,get_Color(g[id1].labs.stitle.color));
    fo1=getFontFromDatabase(g[id1].labs.stitle.font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].labs.stitle.charsize*GeneralPageZoomFactor*universal_font_size_factor);
    this->setText(QString::fromUtf8(g[id1].labs.stitle.alt_plotstring));
    }
delta=fo1.pixelSize()*2-bounding_box.height();
bounding_box.setHeight(2*fo1.pixelSize());
bounding_box.setY(bounding_box.y()-delta/2);
}
else if (edit_type==2)/// Axis Label --> y-Achse-bounding-box stimmt nicht!?
{

int ret=get_axis_label_viewport(id1,id2,&bb1,false,&rot,id4);
if (ret==RETURN_FAILURE)
{
edit_type=-1;//initialization failed
errmsg("Invalid axis label for inline-editing (label viewport not found)");
return;
}

bounding_box=QRect(xconvxlib(bb1.xv1),win_h-xconvxlib(bb1.yv1),win_w,yconvxlib(bb1.yv1)-yconvxlib(bb1.yv2));

//get_graph_viewport(id1,&bb1);
//double rot=getOrientationAngle(0,id1,id2);

pal.setColor(QPalette::Text,get_Color(g[id1].t[id2]->label.color));
fo1=getFontFromDatabase(g[id1].t[id2]->label.font);
fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].t[id2]->label.charsize*GeneralPageZoomFactor*universal_font_size_factor);

//qDebug() << "Axis Rotation=" << rot;

this->setText(QString::fromUtf8(g[id1].t[id2]->label.alt_plotstring));
}
else if (edit_type==3)/// Legend
{
legend l;
get_legend_entry_bb_for_set(id1,id2,&bb1);
bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv1),2*abs(xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1)),abs(xconvxlib(bb1.yv2)-xconvxlib(bb1.yv1)));
bounding_box.setX(bounding_box.x()+2);
bounding_box.setY(bounding_box.y()-4);
get_graph_legend(id1,&l);
pal.setColor(QPalette::Text,get_Color(l.color));
fo1=getFontFromDatabase(l.font);
fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*l.charsize*GeneralPageZoomFactor*universal_font_size_factor);
pal.setColor(QPalette::Base,get_Color(l.boxfillpen.color));
this->setText(QString::fromUtf8(g[id1].p[id2].orig_lstr));
this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}
else if (edit_type==4)/// custom tick
{/// Position der bounding box ist etwas daneben!?
    bb1=g[id1].t[id2]->tloc[id3].bb;
    bounding_box=QRect(xconvxlib(bb1.xv1),yconvxlib(bb1.yv1),2*abs(xconvxlib(bb1.xv2)-xconvxlib(bb1.xv1)),abs(xconvxlib(bb1.yv2)-xconvxlib(bb1.yv1)));
    bounding_box.setX(bounding_box.x()+2);
    bounding_box.setY(bounding_box.y()-4);
    pal.setColor(QPalette::Text,get_Color(g[id1].t[id2]->tl_color));
    fo1=getFontFromDatabase(g[id1].t[id2]->tl_font);
    fo1.setPixelSize(smaller*MAGIC_FONT_SCALE*g[id1].t[id2]->tl_charsize*GeneralPageZoomFactor*universal_font_size_factor);
this->setText(QString::fromUtf8(g[id1].t[id2]->tloc[id3].orig_label));
this->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
}
bounding_box.setHeight(fo1.pixelSize()*2);
this->setFont(fo1);
this->setPalette(pal);
/*qDebug() << "bb: x=(" << bb1.xv1 << "|" << bb1.xv2 << ") y=(" << bb1.yv1 << "|" << bb1.yv2 << ")";*/
//qDebug() << "Rect: x0=" << bounding_box.x() << "|" << bounding_box.y() << " w=" << bounding_box.width() << " h=" << bounding_box.height();
}

void InLineEditor::show_editor(void)
{
    if (edit_type==0)//text-editor
    {
    old_active_value=pstr[id2].active;
    pstr[id2].active=0;
    }
    else if (edit_type==1)//title
    {
        if (id2==0)//main title
        {
        old_active_value=g[id1].labs.title.color;
        g[id1].labs.title.color=getbgcolor();
        }
        else//subtitle
        {
        old_active_value=g[id1].labs.stitle.color;
        g[id1].labs.stitle.color=getbgcolor();
        }
    }
    else if (edit_type==2)//axis-label
    {
    old_active_value=g[id1].t[id2]->label.color;
    g[id1].t[id2]->label.color=getbgcolor();
    }
    else if (edit_type==3)//legend
    {
    /*legend l;
    get_graph_legend(id1,&l);
    old_active_value=l.color;
    l.color=l.boxfillpen.color;
    set_graph_legend(id1,&l);*/

    /*if (old_text) delete[] old_text;
    old_text=new char[strlen(g[id1].p[id2].orig_lstr)+8];
    strcpy(old_text,g[id1].p[id2].orig_lstr);
    SetMemoryToText(g[id1].p[id2].lstr,g[id1].p[id2].orig_lstr,QString("  "));*/
    }
    else if (edit_type==4)//custom_tick
    {
        if (old_text!=NULL) delete[] old_text;
        if (g[id1].t[id2]->tloc[id3].label!=NULL)
        {
        old_text=new char[2+strlen(g[id1].t[id2]->tloc[id3].label)];
        strcpy(old_text,g[id1].t[id2]->tloc[id3].label);
        g[id1].t[id2]->tloc[id3].label[0]='\0';
        }
        else
        {
        old_text=NULL;
        }
    }
    mainWin->mainArea->completeRedraw();
    this->show();
    this->setFocus();
}

void InLineEditor::hide_editor(bool redraw=true)
{
    this->hide();
    if (edit_type==0)//text
    {
    pstr[id2].active=old_active_value;
    }
    else if (edit_type==1)//title
    {
        if (id2==0)//main title
        {
        g[id1].labs.title.color=old_active_value;
        }
        else//subtitle
        {
        g[id1].labs.stitle.color=old_active_value;
        }
    }
    else if (edit_type==2)//axis-label
    {
    g[id1].t[id2]->label.color=old_active_value;
    }
    else if (edit_type==3)//legend
    {
    /*legend l;
    get_graph_legend(id1,&l);
    l.color=old_active_value;
    set_graph_legend(id1,&l);*/
    //SetMemoryToText(g[id1].p[id2].lstr,g[id1].p[id2].orig_lstr,QString(old_text));
    }
    else if (edit_type==4)//custom-tick
    {
        if (old_text!=NULL)
        {
            g[id1].t[id2]->tloc[id3].label = copy_string(g[id1].t[id2]->tloc[id3].label,old_text);//just copy text back
            delete[] old_text;
            old_text=NULL;
        }
    }
    emit(editorClosed());
    if (redraw)
    mainWin->mainArea->completeRedraw();
}

void InLineEditor::finish_editor(void)
{
//qDebug() << "Edit and Hide";
char * c1,*c2;
bool displStd=true;
char dummy[16];
sprintf(dummy,"[G%d]",id1);
hide_editor();
if (edit_type==0)//text
{
SaveObjectData(id2,OBJECT_STRING);
    DynSetMemoryToLineEdit(pstr[id2].s_plotstring,pstr[id2].alt_plotstring,c1,c2,displStd,true,this);
ObjectDataModified(id2,OBJECT_STRING);
}
else if (edit_type==1)//title
{
SaveSingleGraphState(id1,UNDO_APPEARANCE);
    if (id2==0)//title
    DynSetMemoryToLineEdit(g[id1].labs.title.s_plotstring,g[id1].labs.title.alt_plotstring,c1,c2,displStd,true,this);
    else//subtitle
    DynSetMemoryToLineEdit(g[id1].labs.stitle.s_plotstring,g[id1].labs.stitle.alt_plotstring,c1,c2,displStd,true,this);
SingleGraphModified(id1,UNDO_APPEARANCE);
addAditionalDescriptionToLastNode(-1,tr("Graph title changed")+QString(dummy),QString(),-1);
}
else if (edit_type==2)//axis-label
{
SaveTickmarksStatesPrevious(id2,id2,id1,id1);
    DynSetMemoryToLineEdit(g[id1].t[id2]->label.s_plotstring,g[id1].t[id2]->label.alt_plotstring,c1,c2,displStd,true,this);
TickmarksModified(id2,id2,id1,id1);
sprintf(dummy,"[G%d.",id1);
switch (id2)
{
case 0:
strcat(dummy,QObject::tr("X-axis").toLocal8Bit().constData());
break;
case 1:
strcat(dummy,QObject::tr("Y-axis").toLocal8Bit().constData());
break;
case 2:
strcat(dummy,QObject::tr("Alt-X-axis").toLocal8Bit().constData());
break;
default:
strcat(dummy,QObject::tr("Alt-Y-axis").toLocal8Bit().constData());
break;
}
strcat(dummy,"]");
addAditionalDescriptionToLastNode(-1,tr("Axis label edited")+QString(dummy),QString(),-1);
}
else if (edit_type==3)//legend
{
SaveSingleSetStatePrevious(id1,id2,UNDO_APPEARANCE);
    SetMemoryToText(g[id1].p[id2].lstr,g[id1].p[id2].orig_lstr,text().toUtf8());
SingleSetModified(id1,id2,UNDO_APPEARANCE);
sprintf(dummy,"[G%d.S%d]",id1,id2);
addAditionalDescriptionToLastNode(-1,tr("Set legend changed")+QString(dummy),QString(),-1);
}
else if (edit_type==4)//custom-tick
{
    DynSetMemoryToLineEdit(g[id1].t[id2]->tloc[id3].label,g[id1].t[id2]->tloc[id3].orig_label,c1,c2,displStd,true,this);
    /*g[id1].t[id2]->tloc[id3].label = copy_string(g[id1].t[id2]->tloc[id3].label,old_text);
    g[id1].t[id2]->tloc[id3].orig_label = copy_string(g[id1].t[id2]->tloc[id3].orig_label,old_text);*/
    delete[] old_text;
    old_text=NULL;
}
set_dirtystate();
mainWin->mainArea->completeRedraw();
}

void InLineEditor::keyPressEvent(QKeyEvent * event)
{
event->accept();
    if (event->key()==Qt::Key_Escape)
    {
    hide_editor();
    }
    else
    {
    QLineEdit::keyPressEvent(event);
    }
}

void InLineEditor::focusOutEvent(QFocusEvent * event)
{
(void)event;
//qDebug() << "Focus Out";
hide_editor();
}

InLineEditor2::InLineEditor2(QWidget * parent):QGraphicsView(parent)
{
lenEdit=new InLineEditor(0);
scene=new QGraphicsScene(this);
proxy = scene->addWidget(lenEdit);
transpPalette=this->palette();
transpPalette.setColor(QPalette::Base,Qt::transparent);
transpPalette.setColor(QPalette::AlternateBase,Qt::transparent);
transpPalette.setColor(QPalette::Window,Qt::transparent);
transpPalette.setColor(QPalette::Button,Qt::transparent);
transpPalette.setColor(QPalette::Text,Qt::black);

scene->setPalette(transpPalette);
lenEdit->setPalette(transpPalette);
proxy->setPalette(transpPalette);
this->setPalette(transpPalette);

scene->setBackgroundBrush(Qt::transparent);
this->setBackgroundBrush(Qt::transparent);

setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

this->setScene(scene);
this->setFrameStyle(QFrame::NoFrame);
/// QRectF ref=scene->sceneRect();
/// qDebug() << "x=" << ref.x() << " y=" << ref.y() << " w=" << ref.width() << " h=" << ref.height();
hide();
connect(lenEdit,SIGNAL(editorClosed()),this,SLOT(hide()));
connect(lenEdit,SIGNAL(editorClosed()),this,SLOT(resetRotation()));
}

void InLineEditor2::init_editor(int type,int id_1,int id_2,int id_3,int id_4)
{
lenEdit->init_editor(type,id_1,id_2,id_3,id_4);
//qDebug() << "inited editor: x=" << lenEdit->bounding_box.x() << " y=" << lenEdit->bounding_box.y();
}

void InLineEditor2::show_editor(double angle)
{
    int x_shift,y_shift,xp,yp;
    double sa=sin(angle*M_PI/180.0);
    double ca=cos(angle*M_PI/180.0);
    double tmp1,tmp2;

#ifdef MAC_SYSTEM
    #if QT_VERSION >= 0x050000
    x_shift=0;
    y_shift=-1;
    #else
    x_shift=14;
    y_shift=-2;
    #endif
#endif
#ifdef WINDOWS_SYSTEM
    x_shift=2;
    y_shift=0;
#endif
#ifdef LINUX_SYSTEM
    x_shift=2;
    y_shift=0;
#endif

lenEdit->show_editor();
xp=lenEdit->bounding_box.x();
yp=lenEdit->bounding_box.y();

this->setGeometry(mainWin->mainArea->lblBackGr->rect());
scene->setSceneRect(mainWin->mainArea->lblBackGr->rect());

lenEdit->setGeometry(0,0,lenEdit->bounding_box.width(),lenEdit->bounding_box.height());

QRect rec=lenEdit->style()->itemTextRect(lenEdit->fontMetrics(),lenEdit->rect(),lenEdit->alignment(),true,lenEdit->text());
x_shift+=rec.x();
y_shift+=(lenEdit->height()-rec.height())/2+rec.height()-lenEdit->fontMetrics().descent();

proxy->resetTransform();
proxy->setRotation(-angle);

tmp1=ca*x_shift+sa*y_shift;
tmp2=-sa*x_shift+ca*y_shift;

x_shift=rint_2(tmp1);
y_shift=rint_2(tmp2);

qDebug() << "xp=" << xp << " yp(von unten)=" << yp << " yp(von oben)=" << this->height()-yp;
qDebug() << "x_shift=" << x_shift << " y_shift=" << y_shift;
qDebug() << "proxy.x=" << proxy->x() << " proxy.y=" << proxy->y();

/// proxy->moveBy(xp-proxy->x()-x_shift,this->height()-(yp-proxy->y()+y_shift));
/// proxy->moveBy(xp,this->height()-yp);
proxy->setPos(xp-x_shift,this->height()-yp-y_shift);
/*
qDebug() << "rec.x=" << rec.x() << " y=" << rec.y() << " w=" << rec.width() << " h=" << rec.height() << " baseline=" << baseline;
rec=lenEdit->style()->itemTextRect(lenEdit->fontMetrics(),lenEdit->rect(),Qt::AlignLeft,true,lenEdit->text());
qDebug() << "Align Left rec.x=" << rec.x() << " y=" << rec.y() << " w=" << rec.width() << " h=" << rec.height() << " baseline=" << baseline;
qDebug() << "lenEdit.rect=" << lenEdit->rect();

start_pos_x=rec.x();
start_pos_y=rec.y()+baseline;

qDebug() << "TextStart: x=" << start_pos_x << " y=" << start_pos_y;
//lenEdit->getTextMargins(&x,&y2,&x2,&y);
//qDebug() << "textMargin: x=" << x << " y=" << y << " x2=" << x2 << " y2=" << y2;
QPoint textPos=lenEdit->mapToParent(QPoint(start_pos_x,start_pos_y));
x=textPos.x();
y=textPos.y();
qDebug() << "lenEdit: position in parent: x=" << textPos.x() << " y=" << textPos.y();
QPointF textPos2=proxy->mapToParent(textPos);
textPos2=this->mapToScene(x,y);
qDebug() << "in scene: x=" << x << " y=" << y;
//textPos=this->mapFromScene(textPos2);
qDebug() << "proxy_pos: x=" << proxy->scenePos().x() << " y=" << proxy->scenePos().y();
textPos=this->mapFromScene(proxy->pos());
//textPos=textPos2.toPoint();
qDebug() << "proxy: position in Scene: x=" << textPos.x() << " y=" << textPos.y();

textPos=this->mapFromScene(QPoint(x,y));
qDebug() << "proxy: position in Scene: x=" << textPos.x() << " y=" << textPos.y();


textPos=this->mapToParent(textPos);
qDebug() << "this: position in parent: x=" << textPos.x() << " y=" << textPos.y();

QRect rec2=this->mapFromScene(proxy->rect()).boundingRect();
qDebug() << "lenEdit.rect=" << lenEdit->rect();
qDebug() << "proxy.rect  =" << proxy->rect();
qDebug() << "this.rect   =" << this->rect();
qDebug() << "rec2        =" << rec2;
*/

this->show();
this->setFocus();
proxy->setFocus();
lenEdit->setFocus(Qt::MouseFocusReason);
}

void InLineEditor2::resetRotation(void)
{
this->resetTransform();
}

void InLineEditor2::hide_editor(void)
{
lenEdit->hide_editor();
this->hide();
//this->resetTransform();
/*this->rotate(-rot_angle);
rot_angle=0.0;*/
}

void InLineEditor2::finish_editor(void)
{
lenEdit->finish_editor();
this->hide();
//this->resetTransform();
/*this->rotate(-rot_angle);
rot_angle=0.0;*/
}

void InLineEditor2::setRotationAngle(double angle)
{
this->rotate(angle);
}

void InLineEditor2::getTextOffset(int & x_shift,int & y_shift)
{
#ifdef MAC_SYSTEM
    #if QT_VERSION >= 0x050000
    x_shift=0;
    y_shift=0;
    #else
    double s=lenEdit->font().pixelSize();
    /*x_shift=(int)rint_2(-0.16765+0.89191*s);
    y_shift=-(int)rint_2(-0.26912+0.11103*s);*/
    x_shift=(int)rint_2(1.05477*s);//14
    y_shift=-(int)rint_2(-0.0150101*s);//-2;
    #endif
#endif
#ifdef WINDOWS_SYSTEM
    double s=lenEdit->font().pixelSize();
    x_shift=(int)rint_2(-0.144219*s);//2;
    y_shift=0;
#endif
#ifdef LINUX_SYSTEM
    x_shift=1;
    y_shift=0;
#endif
}

void InLineEditor2::init_and_show_editor(int type,int id_1,int id_2, int id_3,int id_4)
{
this->setGeometry(mainWin->mainArea->lblBackGr->rect());
//scene->setSceneRect(mainWin->mainArea->lblBackGr->rect());//important!!!
scene->setSceneRect(0,0,mainWin->mainArea->lblBackGr->width(),mainWin->mainArea->lblBackGr->height());
lenEdit->init_editor(type,id_1,id_2,id_3,id_4);
if (lenEdit->edit_type<0) return;//initialization failed
lenEdit->show_editor();
lenEdit->resize(lenEdit->bounding_box.width(),lenEdit->bounding_box.height());
proxy->setPos(scene->width()/2,scene->height()/2);//for safety
//find the angle
double rot=0.0;
int rot2;
view vp;
QPoint target_center;
QPoint current_center;
//type: 0=text;1=title;2=axis-label;3=legend;4=custom_tick
//type=0 --> id1=graph_nr, id2=text-id
//type=1 --> id1=graph_nr, id2=(0=title,1=sub_title)
//type=2 --> id1=graph_nr, id2=axis-nr(x,y,alt-x,alt-y)
//type=3 --> id1=graph_nr, id2=set_nr
//type=4 --> id1=graph_nr, id2=axis_nr, id3=tick_nr
switch (type)
{
case 0:
rot=getOrientationAngle(1,OBJECT_STRING,id_2);//type=0=axis, type=1=object / nr=axis-nr or object-nr / gno=graph-nr or object-type for objects
break;
case 1://title is never rotated
case 3://legend is never rotated
rot=0.0;
break;
case 2:
(void)get_axis_label_viewport(id_1,id_2,&vp,false,&rot2,id_4);
rot=1.0*rot2;
/*qDebug() << "viewport=" << vp.xv1 << "|" << vp.yv1 << " -- " << vp.xv2 << "|" << vp.yv2;
qDebug() << "pixel_values=" << xconvxlib(vp.xv1) << "|" << yconvxlib(vp.yv1) << " -- " << xconvxlib(vp.xv2) << "|" << yconvxlib(vp.yv2);*/
target_center=QPoint((xconvxlib(vp.xv1)+xconvxlib(vp.xv2))*0.5,(yconvxlib(vp.yv1)+yconvxlib(vp.yv2))*0.5);
//rot=getOrientationAngle(0,id_1,id_2);
break;
case 4:
rot=g[id_1].t[id_2]->tl_angle*180.0/M_PI;
break;
}
//now we position the editor
int xp=lenEdit->bounding_box.x(),yp=lenEdit->bounding_box.y();
    //this->centerOn(xp,yp);
//qDebug() << "Text=" << lenEdit->text() << "Position: xp=" << xp << " y=" << yp << " Rotation=" << rot;
    int x_shift,y_shift;
    double sa=sin(rot*M_PI/180.0);
    double ca=cos(rot*M_PI/180.0);
    double tmp1,tmp2;
/*#ifdef MAC_SYSTEM
    #if QT_VERSION >= 0x050000
    x_shift=0;
    y_shift=-1;
    #else
    x_shift=14;
    y_shift=-2;
    #endif
#endif
#ifdef WINDOWS_SYSTEM
    x_shift=2;
    y_shift=0;
#endif
#ifdef LINUX_SYSTEM
    x_shift=2;
    y_shift=0;
#endif*/

getTextOffset(x_shift,y_shift);

QRect rec=lenEdit->style()->itemTextRect(lenEdit->fontMetrics(),lenEdit->rect(),lenEdit->alignment(),true,lenEdit->text());
x_shift+=rec.x();
y_shift+=(lenEdit->height()-rec.height())/2+rec.height()-lenEdit->fontMetrics().descent();

proxy->resetTransform();
proxy->setRotation(-rot);

tmp1=ca*x_shift+sa*y_shift;
tmp2=-sa*x_shift+ca*y_shift;

x_shift=rint_2(tmp1);
y_shift=rint_2(tmp2);

//qDebug() << "x_shift=" << x_shift << " y_shift=" << y_shift;

if (lenEdit->alignment() & Qt::AlignHCenter)
proxy->moveBy(xp-proxy->x(),yp-proxy->y());
else
proxy->moveBy(xp-proxy->x()-x_shift,yp-proxy->y()-y_shift);

if (type==2)
{
    if (rot==0)
    {
    current_center=QPoint(proxy->x()+0.5*proxy->size().width(),proxy->y()+0.5*proxy->size().height());
    proxy->moveBy(target_center.x()-current_center.x(),target_center.y()-current_center.y());
    }
    else
    {
    y_shift=abs(xconvxlib(vp.yv1)-xconvxlib(vp.yv2));
    current_center=QPoint(proxy->x()+0.5*proxy->rect().height(),proxy->y()+0.5*proxy->rect().width());
    proxy->moveBy(target_center.x()-current_center.x(),-target_center.y()+current_center.y()-y_shift);
    }
/*
qDebug() << "current_center=" << current_center << " size=" << proxy->size() << " rect=" << proxy->rect();
qDebug() << "target_center =" << target_center;
    if (rot==0)
    current_center=QPoint(proxy->x()+0.5*proxy->size().width(),proxy->y()+0.5*proxy->size().height());
    else
    current_center=QPoint(proxy->x()+0.5*proxy->rect().height(),proxy->y()+0.5*proxy->rect().width());
qDebug() << "new current_center=" << current_center;
qDebug() << "proxy.rect=" << proxy->rect();
*/
}

//scene->setSceneRect(0,0,mainWin->mainArea->lblBackGr->width(),mainWin->mainArea->lblBackGr->height());
this->show();
this->setFocus();
proxy->setFocus();
lenEdit->setFocus(Qt::MouseFocusReason);
}

SpinBoxAction::SpinBoxAction(const QString & title1,const QString & title2,QWidget * parent):QWidgetAction(parent)
{
empty=new QWidget(NULL);
spnValue=new QSpinBox(NULL);
spnValue->setRange(100,3000);
spnValue->setSingleStep(100);
lblText1=new QLabel(title1);
lblText2=new QLabel(title2);
layout=new QHBoxLayout();
/*layout->setMargin(0);
layout->setSpacing(0);*/
layout->addWidget(lblText1);
layout->addWidget(spnValue);
layout->addWidget(lblText2);
empty->setLayout(layout);
setDefaultWidget(empty);
}

void LoadBinaryData(int gno,int setno,int settype,char * filename,struct BinReadInstructions inst)
{
    plotarr * p;
    char dummy[256];
    int importcount=0;
    int readbytes;
    int allocated;
    int startcount;
    char * dest;
    int * positions=new int[inst.n_columns_to_read];//positions, where the different colums are in one complete column set
    int length_of_one_column_set=0;

    for (unsigned int i=0;i<inst.column_count;i++)//all columns in a set (to be read or not)
        length_of_one_column_set+=inst.column_type[i].size;

    dest=new char[length_of_one_column_set+8];//prepare space to read one set

    if (inst.read_until_end_is_reached==true)
    {
        startcount=-1;//we will count downwards --> 0 is never reached
    }
    else
    {
        startcount=inst.datasets_to_read;//we will count downwards until zero is reached
    }
    //prepare set for writing
    //int retval=activateset(gno,setno);
    (void)activateset(gno,setno);
    set_dataset_type(gno,setno,settype);
    /*cout << "setlength=" << getsetlength(gno, setno) << endl;
cout << "number of cols=" << dataset_cols(gno, setno) << endl;
cout << "number of cols new settype=" << settype_cols(settype) << endl;
cout << "filename='" << filename << "'" << endl;*/
    p = &g[gno].p[setno];
    allocated=100;
    //prepare some space for the data that is to be read an stored
    p->data.len=0;
    for (unsigned int i=0;i<inst.n_columns_to_read;i++)
    {
        p->data.ex[i] = (double *)xrealloc(p->data.ex[i], allocated*sizeof(double));
        positions[i]=0;
        for (unsigned int j=0;j<i;j++)
            positions[i]+=inst.column_type[j].size;
        //cout << "column_offset[" << i << "]=" << positions[i] << endl;
    }
    ifstream ifi;
    ifi.open(filename);

    if (inst.length_of_header>0)
    {
        ifi.seekg(inst.length_of_header);
    }

    while (startcount!=0)
    {
        ifi.read(dest,length_of_one_column_set);//read one set of columns in one go into dest
        readbytes=(int)(ifi.gcount());
        if (readbytes!=length_of_one_column_set)
        {
            //cout << "error reading datacolumn; " << importcount << " sets already read" << endl;
            sprintf(dummy,"[Information] Stopped reading after import of %d datasets.",importcount);
            stufftext(dummy);
            break;
        }
        else//normal read operation
        {
            startcount--;
            //convert columns into dataset
            for (unsigned int i=0;i<inst.n_columns_to_read;i++)
            {
                switch (inst.column_type[inst.col_numbers_to_read[i]].format)
                {//get format of colum that is to be read
                case COLUMN_SIG_CHAR:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed char*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_CHAR:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned char*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_SIG_SHORT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed short*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_SHORT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned short*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_SIG_INT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed int*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_INT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned int*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_SIG_LONG:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (signed long*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_USIG_LONG:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (unsigned long*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_FLOAT:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (float*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_DOUBLE:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (double*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                case COLUMN_LONG_DOUBLE:
                    p->data.ex[i][importcount]=(double)(
                                *(
                                    (long double*)(
                                        (void*)(dest+positions[i])
                                        )
                                    )
                                );
                    break;
                }
            }
            //check for enough space for the next run
            importcount++;
            if (importcount==allocated)
            {
                allocated+=100;
                for (unsigned int i=0;i<inst.n_columns_to_read;i++)
                {
                    p->data.ex[i] = (double *)xrealloc(p->data.ex[i], allocated*sizeof(double));
                    if (!p->data.ex[i]) cout << "error in reallocation" << endl;
                }
            }
        }
        if (ifi.eof()) break;
    }

    for (unsigned int i=0;i<inst.n_columns_to_read;i++)
        p->data.ex[i] = (double *)xrealloc(p->data.ex[i], importcount*sizeof(double));
    p->data.len=importcount;

    delete[] positions;
}

void GenerateLaTeXCommands(void)
{
    LaTeXCommands.clear();
    equivalentCommands.clear();
    equivalentFont.clear();
}

/*void store_plot_string(plotstr * p,stdLineEdit * led)
{
QString text=led->text();
char * te=new char[text.length()+2];
strcpy(te,text.toLatin1().constData());
p->s = copy_string(p->s, te);
p->alt = copy_string(p->alt, te);
//ENTER LATEX-CONVERSION HERE
delete[] te;
}*/

QString CreateRichTextColorTable(void)
{
    char dummy[32];
    QString table=QString("{\\colortbl;");
    for (int i=0;i<allocated_colors;i++)
    {
        sprintf(dummy,"\\red%d\\green%d\\blue%d;",cmap_table[i].rgb.red,cmap_table[i].rgb.green,cmap_table[i].rgb.blue);
        table+=QString(dummy);
    }
    table+=QString("}");
    return table;
}

//void find_set_ids(char * str,int * nr,int ** startpos,int ** len,int ** gnos,int ** snos)
void find_set_ids(char * str,int * nr,struct FoundSetID ** foundIds)
{
    int strlength=strlen(str);
    int maxnr=strlength/5+8;
    struct FoundSetID * found=new struct FoundSetID[maxnr];
    int index=0,offset=0;//,a,b;
    char * tmpstr=NULL;
    char next_char;
    QStringList list;
    QString original(str);
    original=original.toUpper();
    //cout << "String=#" << original.toLatin1().constData() << "#" << endl;
    QRegExp regex1("G\\d+\\.S\\d+");//what we search for as a regular expression --> we only search for complete set-ids
    while (offset<strlength)
    {
        found[index].pos=regex1.indexIn(original,offset);//find next set-id
        if (found[index].pos==-1)//nothing found
        {
            offset=strlength;
            break;
        }
        list=regex1.capturedTexts();//get the found text
        found[index].len=regex1.matchedLength();//get the length of the found text
        found[index].characteristic=0;//we assume a basic set-id at first
        tmpstr=new char[found[index].len+4];
        strcpy(tmpstr,list.at(0).toLatin1().constData());//copy found text for analysis
        sscanf(tmpstr,"G%d.S%d",&(found[index].gno),&(found[index].sno));

        offset=found[index].pos+found[index].len;
        //cout << "tmpstr=#" << tmpstr <<  "# offset="<< offset << " len=" << strlength << " pos=" << found[index].pos << " einzellaenge=" << found[index].len << endl;
        if (offset<strlength && offset>=0)
        {
            //next_char=tmpstr[strlen(tmpstr)-1];
            next_char=original.at(offset).toLatin1();
            if (next_char=='#')
            {
                found[index].characteristic=2;
                offset++;
                found[index].len++;
            }
            else if (next_char=='$')
            {
                found[index].characteristic=1;
                offset++;
                found[index].len++;
            }
            //cout << "next_char=#" << next_char << "# characteristic=" << found[index].characteristic << endl;
        }

        delete[] tmpstr;
        index++;
    }//end while-loop

    /*for (int i=0;i<index;i++)
{
cout << i << ": pos=" << found[i].pos << " len=" << found[i].len << " G" << found[i].gno << " S" << found[i].sno << endl;
}*/

    *nr=index;
    if (index>0)
    {
        if ((*foundIds)!=NULL)
            delete[] *foundIds;
        *foundIds=new struct FoundSetID[index+1];
        memcpy(*foundIds,found,sizeof(struct FoundSetID)*index);
    }
    delete[] found;
}

void find_graph_ids(char * str,int * nr,struct FoundSetID ** foundIds)
{
    int strlength=strlen(str);
    int maxnr=strlength/5+8;
    struct FoundSetID * found=new struct FoundSetID[maxnr];
    int index=0,offset=0;//,a,b;
    char * tmpstr=NULL,du;
    char next_char;
    QStringList list;
    QString original(str);
    original=original.toUpper();
    QRegExp regex1("\\sG\\d+(?!:\\.|\\S)");//what we search for as a regular expression

    //cout << "String=#" << original.toLatin1().constData() << "# (start=47)" << endl;

    while (offset<strlength)
    {
        found[index].pos=regex1.indexIn(original,offset)+1;//find next set-id
        if (found[index].pos<=0)//nothing found
        {
            offset=strlength;
            break;
        }
        list=regex1.capturedTexts();//get the found text
        found[index].len=regex1.matchedLength()-1;//get the length of the found text
        found[index].characteristic=0;
        tmpstr=new char[found[index].len+4];
        strcpy(tmpstr,list.at(0).toLatin1().constData());//copy found text for analysis
        //cout << "#" << tmpstr << "#" << endl;
        sscanf(tmpstr,"%cG%d",&du,&(found[index].gno));
        found[index].sno=-1;
        delete[] tmpstr;
        offset+=found[index].pos+found[index].len;
        if (offset<strlength && offset>=0)
        {
            next_char=original.at(offset).toLatin1();
            //cout << "next_char=#" << next_char << "#" << endl;
            if (next_char=='#')
            {
                found[index].characteristic=2;
                offset++;
                found[index].len++;
            }
            else if (next_char=='$')
            {
                found[index].characteristic=1;
                offset++;
                found[index].len++;
            }
        }
        index++;
    }//end while-loop

    /*for (int i=0;i<index;i++)
{
cout << i << ": pos=" << found[i].pos << " len=" << found[i].len << " G" << found[i].gno << " S" << found[i].sno << endl;
}*/

    *nr=index;
    if (index>0)
    {
        delete[] *foundIds;
        *foundIds=new struct FoundSetID[index+1];
        memcpy(*foundIds,found,sizeof(struct FoundSetID)*index);
    }
    delete[] found;
}

//void postprocess_found_set_ids(int nr,int * gnos,int * snos,int * nr_unique_ids,int * id_color,int * unique_id)
void postprocess_found_set_ids(int nr,struct FoundSetID ** foundIds,int * nr_unique_ids,int ** unique_id)
{
    int cur_col;
    int * uni_ids=new int[nr+2];
    *nr_unique_ids=0;
    for (int i=0;i<nr;i++)
    {
        (*foundIds)[i].color=-1;//no valid color
        uni_ids[i]=-1;//no valid number
    }
    for (int i=0;i<nr;i++)//look through every set-id
    {
        if ((*foundIds)[i].color>-1) continue;//we are done with this id already
        if (is_set_active((*foundIds)[i].gno,(*foundIds)[i].sno)==TRUE)//the set already exists-->we take the color from this set
        {
            cur_col=g[(*foundIds)[i].gno].p[(*foundIds)[i].sno].linepen.color;
        }
        else
        {
            cur_col = (i+2) % number_of_colors();
        }
        (*foundIds)[i].color=cur_col;
        uni_ids[(*nr_unique_ids)++]=i;
        for (int j=i+1;j<nr;j++)//compare with every other set-id
        {
            if ((*foundIds)[i].gno==(*foundIds)[j].gno && (*foundIds)[i].sno==(*foundIds)[j].sno)
            {
                (*foundIds)[j].color=cur_col;
            }
        }
    }
    delete[] *unique_id;
    (*unique_id)=new int[*nr_unique_ids+2];
    memcpy(*unique_id,uni_ids,sizeof(int)*(*nr_unique_ids));
    delete[] uni_ids;
}

QString ColorToHtml(int nr)
{
    static char dummy[16];
    sprintf(dummy,"%02x%02x%02x",cmap_table[nr].rgb.red,cmap_table[nr].rgb.green,cmap_table[nr].rgb.blue);
    return QString("<span style=\"color:#") + QString(dummy) + QString("\">");
}

QString ColorHtmlEnd(void)
{
    return QString("<\\span>");
}

QString PaintSetIds(char * str,int nr,struct FoundSetID * setIds)
{
    /*QString tmp=QString("{\\f1 \n")+RTColorTable+QString("\n\\cf2 ");
tmp+=QString("Dies \\cf3 ist \\cf4 ein \\cf5 Test\\cf2 !");
return tmp+QString("}\n");*/
    QString tmp;
    QString original(str);
    char dummy[32];
    int pos=0;
    tmp+=ColorToHtml(1);
    for (int i=0;i<nr;i++)
    {
        tmp+=original.mid(pos,setIds[i].pos-pos);
        tmp+=ColorHtmlEnd();
        tmp+=ColorToHtml(setIds[i].color);
        sprintf(dummy,"G%d.S%d",setIds[i].gno,setIds[i].sno);
        tmp+=QString(dummy);
        tmp+=ColorHtmlEnd();
        tmp+=ColorToHtml(1);
        pos=setIds[i].pos+setIds[i].len;
    }
    tmp+=original.mid(pos,strlen(str));
    tmp+=ColorHtmlEnd();
    return tmp;
}

QString ReplaceSetIds(char * str,int nr,struct FoundSetID * setIds)
{
    if (nr<1) return QString(str);//no ids found, no ids to replace --> replace original
    QString original=QString(str);
    QString result;
    char new_set_id[32];
    int pos=0;
    result.clear();
    for (int i=0;i<nr;i++)//replace every set-id
    {
        if (setIds[i].sno<0) continue;//no real set-id
        sprintf(new_set_id,"G%d.S%d",setIds[i].repl_gno,setIds[i].repl_sno);
        result+=original.mid(pos,setIds[i].pos-pos);
        result+=QString(new_set_id);
        pos=setIds[i].pos+setIds[i].len;
    }
    result+=original.mid(pos);
    return result;
}

QString ReplaceGraphIds(char * str,int nr,struct FoundSetID * setIds)
{
    if (nr<1) return QString(str);
    QString original=QString(str);
    QString result;
    char new_set_id[32];
    int pos=0;
    result.clear();
    for (int i=0;i<nr;i++)//replace every Graph-id
    {
        if (setIds[i].sno>=0) continue;//no graph id, but real set id
        sprintf(new_set_id,"G%d",setIds[i].repl_gno);
        result+=original.mid(pos,setIds[i].pos-pos);
        result+=QString(new_set_id);
        pos=setIds[i].pos+setIds[i].len;
    }
    result+=original.mid(pos);
    return result;
}

void replace_single_formula_tokens(QString old_formula,QString & new_formula)
{
QString n_value;
new_formula=old_formula;
//cout << "tokens=" << nr_of_single_f_tokens << endl;
    for (int i=0;i<nr_of_single_f_tokens;i++)
    {
        switch (formula_tokens[i].type)
        {
        case 0://integer-counter
        n_value=QString::number(formula_tokens[i].get_counter_value());
            break;
        case 1://double-value
        n_value=QString::number(formula_tokens[i].get_d_value());
            break;
        default:
        n_value=QString("0");
            break;
        }
    n_value=QString("(")+n_value+QString(")");
    //cout << formula_tokens[i].representation << " type=" << (formula_tokens[i].type==0?"COUNTER":"VALUE") << " --> n_value=" << n_value.toLatin1().constData() << endl;
    new_formula.replace(formula_tokens[i].representation,n_value);
    //cout << "replaced" << endl;
    }
}

int find_single_formula_token_in_list(int anz,class single_formula_token * tokenlist,char * token)
{
int ret=-1;
for (int i=0;i<anz;i++)
{
    if (strcmp(tokenlist[i].representation,token)==0)
    {
    ret=i;
    break;
    }
}
return ret;
}

int find_next_single_formula_token(char * formula,class single_formula_token * n_token)//finds the first token in a formula, returns the token and its position in the formula
{
static QRegExp exp1("N{1}\\d+[\\#\\$]{1}");
int ret=exp1.indexIn(QString(formula));
    if (ret!=-1)
    {
    QStringList list=exp1.capturedTexts();
    QString st=list.at(0);
    n_token->reset_token(st.toLatin1().data());
    }
return ret;
}

int find_all_single_formula_tokens(char * formula,class single_formula_token ** n_token)
{
int anz=0;
int anz_loc=10,pos,ret;
//int len=strlen(formula);
if ((*n_token)!=NULL) delete[] (*n_token);
(*n_token)=new class single_formula_token[anz_loc];
pos=0;
ret=1;
while (ret>=0)
{
ret=find_next_single_formula_token(formula+pos,(*n_token)+anz);
    if (ret>=0)//new token found
    {
    anz++;
    pos+=ret+1;
    ret=find_single_formula_token_in_list(anz,*n_token,(*n_token)[anz-1].representation);
    if (ret==-1)
    {
    cout << "Error! Single token not found in list." << endl;
    }
    else if (ret!=anz-1)//does already exist
    {
    anz--;
    }
        if (anz==anz_loc)
        {
        class single_formula_token * tm=new class single_formula_token[anz_loc+10];
            for (int j=0;j<anz;j++)
            {
            tm[j]=(*n_token)[j];
            }
        delete[] (*n_token);
        *n_token=tm;
        anz_loc+=10;
        }
    }
}
return anz;
}

int find_single_formula_tokens_in_list(QStringList list,int & nr_of_tokens_found,class single_formula_token ** n_token)
{
nr_of_tokens_found=0;
class single_formula_token * f_token=new class single_formula_token[4];
char * cur_command=NULL;
int single_found=0,pos;
    if ((*n_token)!=NULL) delete[] (*n_token);
for (int i=0;i<list.length();i++)
{
cur_command=new char[list.at(i).length()+2];
strcpy(cur_command,list.at(i).toLatin1().constData());
single_found=find_all_single_formula_tokens(cur_command,&f_token);//find all tokens in a single command
    for (int j=0;j<single_found;j++)//look up all found tokens
    {
    pos=find_single_formula_token_in_list(nr_of_tokens_found,(*n_token),f_token[j].representation);//find the found tokens in the previously found list
        if (pos>=0)//this tokens has previously been found --> shorten the list
        {
            for (int k=j+1;k<single_found;k++)
            f_token[k-1]=f_token[k];
        single_found--;
        }
    }
    if (single_found>0)//add the new found tokens to the list
    {
    class single_formula_token * f_token2=new class single_formula_token[single_found+nr_of_tokens_found+2];
        for (int k=0;k<nr_of_tokens_found;k++)//copy old ones
        f_token2[k]=(*n_token)[k];
        for (int k=0;k<single_found;k++)//add new found ones
        f_token2[nr_of_tokens_found+k]=f_token[k];
    delete[] (*n_token);
        (*n_token)=f_token2;
    nr_of_tokens_found+=single_found;
    }
delete[] cur_command;
}

return nr_of_tokens_found;
}

single_formula_token::single_formula_token(char * token)
{
type=2;
representation=NULL;
    if (token!=NULL)
    reset_token(token);
i_counter=0;
d_value=0.0;
}

single_formula_token::~single_formula_token()
{
    if (representation!=NULL)
    delete[] representation;
}

void single_formula_token::initialize(void * value,int n_type)
{
    if (n_type==-1)//no change in type
    {
        if (type==0)//counter
        {
        i_counter=*((int*)value);
        }
        else if (type==1)//double value
        {
        d_value=*((double*)value);
        cout << "d_val=" << d_value << endl;
        }
    }
    else if (n_type==0)
    {
        type=n_type;
        i_counter=*((int*)value);
    }
    else if (n_type==1)
    {
        type=n_type;
        d_value=*((double*)value);
    }
    else//make it invalid
    {
        type=n_type;
    }
}

void single_formula_token::changeCounter(int delta)
{
i_counter+=delta;
}

void single_formula_token::setValueToFormula(char * formula)
{
double r_d_value;
int ret=std_evalexpr(formula,&r_d_value);
    if (ret==RETURN_SUCCESS)
    d_value=r_d_value;
}

int single_formula_token::get_counter_value(void)
{
return i_counter;
}

void single_formula_token::raise_counter(void)
{
i_counter++;
}

void single_formula_token::lower_counter(void)
{
i_counter--;
}

double single_formula_token::get_d_value(void)
{
return d_value;
}

void single_formula_token::reset_token(char * token)
{
if (representation!=NULL) delete[] representation;
    if (token!=NULL)
    {
    representation=new char[strlen(token)+2];
    strcpy(representation,token);
       if (representation[strlen(representation)-1]=='#')//counter
       type=0;
       else if (representation[strlen(representation)-1]=='$')//a double value
       type=1;
    //cout << (type==0?"COUNTER":"VALUE") << " reseting token to " << representation << endl;
    }
    else
    {
    representation=NULL;
    type=2;
    }
}

class single_formula_token & single_formula_token::operator=(class single_formula_token & a)
{
type=a.type;
if (representation!=NULL) delete[] representation;
representation=new char[strlen(a.representation)+1];
strcpy(representation,a.representation);
i_counter=a.i_counter;
d_value=a.d_value;
return *this;
}

//the following function goes throught text and tries to find the first set- or graph-id and returns what has been found (an ID), important is also the length len for the later replacement; ID-Type is returned
int searchTextForFirstID(QString text,int start,int & pos,int & g_no,int & s_no,char & control1,char & control2,char & col,int & leng)
{
/// Reminder on what special ID-commands mean:
/// G0.S0N --> generate a new set in G0
/// G0N.S0 / G0N.S0N --> generate a new graph (and a new set)
/// G0.S0$ --> do no replacing --> always use G0.S0
/// G0.S0# --> use G0.S0 the first time / G0.S1 the second time / G0.S2 the third time ... G0.S(k-1) the k-th time
/// G# --> just replace with graph-id (depends on selection) --> e.g. 0
/// S# --> just replace with set-id (depends on selection) --> e.g. 0
int ret=ID_NONE,pos2;
QStringList foundList;
QString foundString;
char * c_found_string=NULL;
//for the following regular expressions '.X' or '.Y' at the end is optional
QRegExp RegularSetID("[Gg]{1}\\d+\\.{1}[Ss]\\d+\\.?[XYxy]?");//A complete regular set-ID
QRegExp NewGraphStdSetID("[Gg]{1}\\d+[N\\#\\$]{1}\\.{1}[Ss]{1}\\d+\\.?[XYxy]?");//New GraphID, regular set-ID
QRegExp StdGraphNewSetID("[Gg]{1}\\d+\\.{1}[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");//Regular GraphID, new set-ID
QRegExp NewGraphNewSetID("[Gg]{1}\\d+[N\\#\\$]{1}\\.{1}[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");//New GraphID, new set-ID
QRegExp SetIDOnly("[Ss]{1}\\d+\\.?[XYxy]?");//set-ID only
QRegExp NewSetIDOnly("[Ss]{1}\\d+[N\\#\\$]{1}\\.?[XYxy]?");//New set-ID only
QRegExp GraphIDOnly("[Gg]{1}\\d+(?!(\\.|N|\\#|\\$))");//Graph-ID only (not followed by anything else (especially no '.') )
QRegExp NewGraphIDOnly("[Gg]{1}\\d+[N\\#\\$]{1}(?!(\\.|N|\\#|\\$))");//New set-ID only (not followed by anything else (especially no '.') )
QRegExp ColIDOnly("[XYxy]{1}\\d?");
QRegExp GraphNrOnly("[Gg]{1}\\#{1}(?!(\\.|N|\\#|\\$))");
QRegExp SetNrOnly("[Ss]{1}\\#{1}(?!(\\.|N|\\#|\\$))");
QRegExp SpecialFormula("\\#{1}QTGRACE_SPECIAL");
pos=-1;//nothing found yet
leng=0;
g_no=s_no=-1;
control1=control2=col='\0';
foundString.clear();
foundList.clear();

pos2=SpecialFormula.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=SpecialFormula.capturedTexts();
foundString=foundList.at(0);
ret=ID_SPECIAL_COMMAND;
}

pos2=NewGraphNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=NewGraphNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGNS;
}

pos2=NewGraphStdSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=NewGraphStdSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGSS;
}

pos2=StdGraphNewSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=StdGraphNewSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_SGNS;
}

pos2=RegularSetID.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=RegularSetID.capturedTexts();
foundString=foundList.at(0);
ret=ID_REGULAR_COMPLETE;
}

pos2=NewSetIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=NewSetIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_NSET_ONLY;
}

pos2=SetIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=SetIDOnly.capturedTexts();
qDebug() << foundList;
foundString=foundList.at(0);
ret=ID_SET_ONLY;
}

pos2=NewGraphIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=NewGraphIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_NGRAPH_ONLY;
}

pos2=GraphIDOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=GraphIDOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_GRAPH_ONLY;
}

pos2=GraphNrOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=GraphNrOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_GRAPH_NR_ONLY;
}

pos2=SetNrOnly.indexIn(text,start);
if (pos2>=0 && (pos<0 || pos2<pos))
{
pos=pos2;
foundList=SetNrOnly.capturedTexts();
foundString=foundList.at(0);
ret=ID_SET_NR_ONLY;
}

pos2=ColIDOnly.indexIn(text,start);//if pos2>0 there may be a singular X or Y
//qDebug() << "column found: pos2=" << pos2 << " pos=" << pos;
if (pos2>=0 && (pos<0 || pos2<pos))
{
//qDebug() << "Accept Column maybe";
char testc1,testc2;
bool to_ignore=false;
    if (pos2==0)//an X or Y at the beginning
    {
        if (text.length()>1)
        {
        testc2=text.at(pos2+1).toLatin1();
            if (testc2=='_' || isalpha(testc2))//ignore (|| isdigit(testc2))
            {
            to_ignore=true;
            }
            else if (isdigit(testc2))
            {
            control1=testc2;
            }
        }//else=only character --> is ok
    }
    else if (pos2>0 && pos2<text.length()-1)//X or Y somewhere in the text but not at beginning or end)
    {
    testc1=text.at(pos2-1).toLatin1();//character before
    testc2=text.at(pos2+1).toLatin1();//character after
        if (testc1=='_' || testc1=='.' || testc2=='_' || isalpha(testc1) || isdigit(testc1) || isalpha(testc2))//ignore
        {
        to_ignore=true;
        }
        else if (isdigit(testc2))
        {
        control1=testc2;
        }
    }
    else if (pos2==text.length()-1)//last position (but not first, because this is the first case)
    {
    testc1=text.at(pos2-1).toLatin1();
        if (testc1=='_' || testc1=='.' || isalpha(testc1) || isdigit(testc1))//ignore
        {
        to_ignore=true;
        }
    }
//qDebug() << "foundString=" << foundString;
//qDebug() << "pos2=" << pos2 << " text=" << text;
        if (to_ignore==false)
        {
        foundList=ColIDOnly.capturedTexts();
        foundString=foundList.at(0);
        col=text.at(pos2).toUpper().toLatin1();
        //col=foundString.at(0).toLatin1();
        ret=ID_COLUMN_ONLY;
        pos=pos2;
        }
        else//we found something, that does not fit the description --> go on searching
        {
        return searchTextForFirstID(text,pos2+1,pos,g_no,s_no,control1,control2,col,leng);
        }
}
//qDebug() << "foundString=" << foundString << " col=" << col;
leng=0;
g_no=s_no=-1;
if (ret!=ID_NONE)//we found something --> fill in all informations
{
leng=foundString.length();
c_found_string=new char[8+foundString.length()*2];
strcpy(c_found_string,foundString.toUpper().toLatin1().constData());//we are about to read the entries
int number_of_dots=foundString.count(QString("."));
char tmp_str1[8];
char * parameters;
//qDebug() << "c_found_string=" << c_found_string;
    switch (ret)
    {
    default:
    case ID_REGULAR_COMPLETE://G0.S0.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d.S%d",&g_no,&s_no);
        else
        sscanf(c_found_string,"G%d.S%d.%c",&g_no,&s_no,&col);
    break;
    case ID_NGSS://G0N.S0.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d%c.S%d",&g_no,&control1,&s_no);
        else
        sscanf(c_found_string,"G%d%c.S%d.%c",&g_no,&control1,&s_no,&col);
    break;
    case ID_SGNS://G0.S0N.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d.S%d%c",&g_no,&s_no,&control2);
        else
        sscanf(c_found_string,"G%d.S%d%c.%c",&g_no,&s_no,&control2,&col);
    break;
    case ID_NGNS://G0N.S0N.Y
        if (number_of_dots==1)
        sscanf(c_found_string,"G%d%c.S%d%c",&g_no,&control1,&s_no,&control2);
        else
        sscanf(c_found_string,"G%d%c.S%d%c.%c",&g_no,&control1,&s_no,&control2,&col);
    break;
    case ID_SET_ONLY://S0.Y
        if (number_of_dots==0)
        sscanf(c_found_string,"S%d",&s_no);
        else
        sscanf(c_found_string,"S%d.%c",&s_no,&col);
    break;
    case ID_NSET_ONLY://S0N.Y
        if (number_of_dots==0)
        sscanf(c_found_string,"S%d%c",&s_no,&control2);
        else
        sscanf(c_found_string,"S%d%c.%c",&s_no,&control2,&col);
    break;
    case ID_GRAPH_ONLY://G0
    sscanf(c_found_string,"G%d",&g_no);
    break;
    case ID_NGRAPH_ONLY://G0N
    sscanf(c_found_string,"G%d%c",&g_no,&control1);
    break;
    case ID_COLUMN_ONLY://X or Y
    ;//everthing done before
//qDebug() << "Column=" << col << " Found String=" << foundString << " len=" << leng;
        if (foundString.length()>1)
        {
        sprintf(tmp_str1,"%c",foundString.at(1).toLatin1());
//qDebug() << "ColumnNumber=" << tmp_str1;
        col+=atoi(tmp_str1);
        }
//qDebug() << "NEW Column=" << (int)col;
    break;
    case ID_GRAPH_NR_ONLY://G#
//qDebug() << "GraphNumber=" << foundString;
    break;
    case ID_SET_NR_ONLY://S#
//qDebug() << "SetNumber=" << foundString;
    break;
    case ID_SPECIAL_COMMAND:
    leng=text.length();
    delete[] c_found_string;
    c_found_string=new char[8+text.length()*2];
    strcpy(c_found_string,text.toLatin1().constData());
    control1=(char)containsSpecialCommand(c_found_string,&parameters);
//qDebug() << "Special Command Found (" << (int)control1 << "): " << text;
    break;
    }
}
if (c_found_string!=NULL) delete[] c_found_string;
return ret;
}

QString createCompleteID(int type,int n_gno,int n_sno,int o_gno,int o_sno,char control1,char control2,char col)
{
    (void)o_gno;
    (void)control1;
QString ret;
ret.clear();
switch (type)
{
default:
case ID_REGULAR_COMPLETE://G0.S0.Y
case ID_NGSS://G0N.S0.Y
case ID_SGNS://G0.S0N.Y
case ID_NGNS://G0N.S0N.Y
case ID_GRAPH_ONLY://G0
case ID_NGRAPH_ONLY://G0N
case ID_GRAPH_NR_ONLY://G#
case ID_SET_NR_ONLY://S#
break;
case ID_SET_ONLY://S0.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(o_sno);
break;
case ID_NSET_ONLY://S0N.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(o_sno)+QString(control2);
break;
case ID_COLUMN_ONLY://X or Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
    qDebug() << "complete (column only): col=" << col;
break;
}
if (ret.length()>0 && (col=='X' || col=='Y'))
{
ret+=QString(".")+QString(col);
}
return ret;
}

QString createReplacementID(int type, int n_gno, int n_sno, int o_gno, int o_sno, char control1, char control2, char col)
{
QString ret;
ret.clear();
switch (type)
{
default:
case ID_REGULAR_COMPLETE://G0.S0.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);/// ok
break;
case ID_NGSS://G0N.S0.Y
    if (control1=='$')//no replacing
    {
    ret=QString("G")+QString::number(o_gno)+QString(".S")+QString::number(n_sno);/// ok
    }
    else if (control1=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_gno
    {
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
    }
break;
case ID_SGNS://G0.S0N.Y
    if (control2=='$')//no replacing
    {
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(o_sno);/// ok
    }
    else if (control2=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_sno
    {
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
    }
break;
case ID_NGNS://G0N.S0N.Y
    if (control1=='$')//no replacing
    {
    ret=QString("G")+QString::number(o_gno);/// ok
    }
    else if (control1=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_gno + o_sno
    {
    ret=QString("G")+QString::number(n_gno);
    }
    if (control2=='$')//no replacing
    {
    ret+=QString(".S")+QString::number(o_sno);/// ok
    }
    else if (control2=='#')//count number upwards after every use
    {
    ;
    }
    else//generate o_gno + o_sno
    {
    ret+=QString(".S")+QString::number(n_sno);
    }
break;
case ID_SET_ONLY://S0.Y
    ret=QString("G")+QString::number(n_gno)+QString(".S")+QString::number(n_sno);
break;
case ID_NSET_ONLY://S0N.Y
    ret=QString("G")+QString::number(n_gno);
    /// generate s_gno
    ret+=QString("S")+QString::number(o_sno);
break;
case ID_GRAPH_ONLY://G0
    ret=QString("G")+QString::number(n_gno);
break;
case ID_NGRAPH_ONLY://G0N
    if (control1=='$')//no replacing
    {
    ret=QString("G")+QString::number(o_gno);/// ok
    }
    else if (control1=='#')//count number upwards after every use
    {
    ;
    }
    else
    {
    ret=QString("G")+QString::number(n_gno);
    }
    /// generate n_gno
    ret=QString("G")+QString::number(o_gno);
break;
case ID_GRAPH_NR_ONLY://G#
    ret=QString::number(n_gno);
break;
case ID_SET_NR_ONLY://S#
    ret=QString::number(n_sno);
break;
}
if (col=='X' || col=='Y')
{
ret+=QString(".")+QString(col);
}
return ret;
}

formula_to_process::formula_to_process(void)
{
len_orig_formula=0;
original_formula.clear();
//when a formula is initialized, it is analysed for tokens - for easier replacement
clear_tokens();
}

formula_to_process::~formula_to_process()
{
clear_tokens();
}

void formula_to_process::process_single_string(QString text,QList<int> & t_startpos,QList<int> & t_type,QList<int> & t_len,QList<int> & t_id1,QList<int> & t_id2,QList<char> & t_control1,QList<char> & t_control2,QList<int> & t_column)
{
int f_ret,f_pos,g_no,s_no,st_len;
char control1,control2,col;
int st_pos=0;
t_startpos.clear();
t_type.clear();
t_len.clear();
t_id1.clear();
t_id2.clear();
t_control1.clear();
t_control2.clear();
t_column.clear();
//qDebug() << "Process single string=" << text;
    while ( (f_ret=searchTextForFirstID(text,st_pos,f_pos,g_no,s_no,control1,control2,col,st_len))!=ID_NONE )//as long as we find something
    {
    t_startpos << f_pos;
    t_type << f_ret;
    t_len << st_len;
    t_id1 << g_no;
    t_id2 << s_no;
    t_control1 << control1;
    t_control2 << control2;
    t_column << (int)(col-'X');
//qDebug() << "COLUMN=" << t_column.last();
    st_pos=f_pos+st_len;
    }
}

bool formula_to_process::has_fit_formula_command(QString f)
{
QString text=f;
remove_beginning_whitespaces(text);
text=text.toUpper();
return text.contains(QString("FIT FORMULA"));
}

int formula_to_process::is_comment(QString f)
{
QString text=f;
remove_beginning_whitespaces(text);
if (text.length()<1) return 0;
bool is_special=text.contains(QString("QTGRACE_SPECIAL"));
bool starts_with_comment=(text.at(0).toLatin1()=='#'?true:false);
return is_special==true?2:(starts_with_comment==true?1:0);
}

void formula_to_process::init_formula(QString f)//clear everything and enter new formula
{
clear_tokens();
//original_formula=f.toUpper();//important for recognizing ids
original_formula=f;
len_orig_formula=original_formula.length();
int sub_start=original_formula.indexOf("\"",0);
int sub_end=original_formula.indexOf("\"",sub_start+1);
int current_pos=0,offset=0,comment_status=0;
bool quotes_found=false,fit_command=false;
QList<int> t_startpos;
QList<int> t_type;
QList<int> t_len;
QList<int> t_id1;
QList<int> t_id2;
QList<char> t_control1;
QList<char> t_control2;
QList<int> t_column;
QString sub_left,sub_right,sub_inner;//we have to split in parts inside and outside quotations
QString sub_left1,sub_right1;//we split each part again to look for '=' --> lhs/rhs
QStringList subformulas;
QList<int> suboffsets;
QStringList subformulas2;
QList<int> suboffsets2;
//qDebug() << "Formula to init=" << original_formula;
get_List_of_Subformulas(original_formula,subformulas,suboffsets);/// splitting formulas with ';' between commands
//qDebug() << "Number of Subformulas=" << subformulas.length();
for (int ijk=0;ijk<subformulas.length();ijk++)
{
fit_command=has_fit_formula_command(subformulas.at(ijk));
comment_status=is_comment(subformulas.at(ijk));
if (comment_status==1) continue;//ignore this subcommand if it contains a comment or a special command
//qDebug() << suboffsets.at(ijk) << " Subformula=" << subformulas.at(ijk);
sub_start=subformulas.at(ijk).indexOf("\"",0);
sub_end=subformulas.at(ijk).indexOf("\"",sub_start+1);
offset=suboffsets.at(ijk);
    if (sub_start==sub_end)
    {
        quotes_found=false;
    sub_left.clear();
    sub_right.clear();
    sub_start=-1;
    sub_end=subformulas.at(ijk).length();
    sub_inner=subformulas.at(ijk);
    }
    else
    {
        quotes_found=true;
    sub_left=subformulas.at(ijk).left(sub_start);
    sub_inner=subformulas.at(ijk).mid(sub_start+1,sub_end-sub_start-1);
    sub_right=subformulas.at(ijk).mid(sub_end+1,-1);
    }
//sub_start and sub_end are not included in sub_inner
/*qDebug() << "Formula to analyze:" << subformulas.at(ijk);
qDebug() << "left=#" << sub_left << "#";
qDebug() << "inner=#" << sub_inner << "#";
qDebug() << "right=#" << sub_right << "#";*/

/// Left part = only part if there is no "" inside the formula
int equals_pos=sub_left.indexOf("=");
//offset=0;//we start at the beginning
if (equals_pos<0)//no '=' in left part
{
current_pos=0;
process_single_string(sub_left,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Left Part contains NO '='. Found tokens=" << t_startpos.length();
        for (int i=0;i<t_startpos.length();i++)
        {
        //qDebug() << "lokal_pos=" << t_startpos.at(i);
        token_startpos << t_startpos.at(i)+offset;//absolute position in the original string
        token_type << t_type.at(i);//the type of the id
        token_len << t_len.at(i);//the length of the id (for replacing later)
        token_id1 << t_id1.at(i);
        token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
        token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
        token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
        token_control2 << t_control2.at(i);//control-character for Set-ID
        token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
        }
}
else//'=' in left part --> split left part
{
sub_left1=sub_left.left(equals_pos);
sub_right1=sub_left.mid(equals_pos+1,-1);
current_pos=0;
process_single_string(sub_left1,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Left Part contains '='. Found tokens (left)=" << t_startpos.length();
//qDebug() << "left1= #" << sub_left1 << "#";
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset;//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    }
//qDebug() << "right1=#" << sub_right1 << "#";
current_pos=1;
process_single_string(sub_right1,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Found tokens (right)=" << t_startpos.length();
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset+sub_left1.length()+1;//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    //qDebug() << "t_column=" << token_column;
    }
}
    if (quotes_found==false)//no '\"' found
    offset+=sub_left.length();
    else
    offset+=sub_left.length()+1;//+1 because of the '"' at the beginning
//qDebug() << "offset=start_inner=" << offset;
/// Inner part
get_List_of_Subformulas(sub_inner,subformulas2,suboffsets2);/// splitting formulas with ';' between commands
for (int lmn=0;lmn<subformulas2.length();lmn++)
{
equals_pos=subformulas2.at(lmn).indexOf("=");
if (equals_pos<0)//no '=' in inner part
{
current_pos=0;
process_single_string(subformulas2.at(lmn),t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Inner Part contains NO '='. Found tokens=" << t_startpos.length();
        for (int i=0;i<t_startpos.length();i++)
        {
        //qDebug() << "lokal_pos=" << t_startpos.at(i);
        token_startpos << t_startpos.at(i)+offset+suboffsets2.at(lmn);//absolute position in the original string
        token_type << t_type.at(i);//the type of the id
        token_len << t_len.at(i);//the length of the id (for replacing later)
        token_id1 << t_id1.at(i);
        token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
        token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
        token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
        token_control2 << t_control2.at(i);//control-character for Set-ID
        token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
        }
}
else//'=' in inner part --> split inner part
{
sub_left1=subformulas2.at(lmn).left(equals_pos);
sub_right1=subformulas2.at(lmn).mid(equals_pos+1,-1);
current_pos=0;

if (fit_command==false)
{
process_single_string(sub_left1,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Inner Part contains '='. Found tokens (left)=" << t_startpos.length();
//qDebug() << "left1= #" << sub_left1 << "#";
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset+suboffsets2.at(lmn);//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    }
//qDebug() << "right1=#" << sub_right1 << "#";
current_pos=1;
process_single_string(sub_right1,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Found tokens (right)=" << t_startpos.length();
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset+sub_left1.length()+1+suboffsets2.at(lmn);//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    }
}

}
}//end (lmn)-loop through subformulas in inner part
    if (quotes_found==false)//no '\"' found
    offset+=sub_inner.length();
    else
    offset+=sub_inner.length()+1;//+1 because of the '"' at the beginning
//qDebug() << "offset=start_right=" << offset;
/// Right part
equals_pos=sub_right.indexOf("=");
if (equals_pos<0)//no '=' in left part
{
current_pos=0;
process_single_string(sub_right,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Right Part contains NO '='. Found tokens=" << t_startpos.length();
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset;//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    }
}
else//'=' in left part --> split left part
{
sub_left1=sub_right.left(equals_pos);
sub_right1=sub_right.mid(equals_pos+1,-1);
current_pos=0;
process_single_string(sub_left1,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Right Part contains '='. Found tokens (left)=" << t_startpos.length();
//qDebug() << "left1= #" << sub_left1 << "#";
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset;//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    }
//qDebug() << "right1=#" << sub_right1 << "#";
current_pos=1;
process_single_string(sub_right1,t_startpos,t_type,t_len,t_id1,t_id2,t_control1,t_control2,t_column);
//qDebug() << "Found tokens (right)=" << t_startpos.length();
    for (int i=0;i<t_startpos.length();i++)
    {
    //qDebug() << "lokal_pos=" << t_startpos.at(i);
    token_startpos << t_startpos.at(i)+offset+sub_left1.length()+1;//absolute position in the original string
    token_type << t_type.at(i);//the type of the id
    token_len << t_len.at(i);//the length of the id (for replacing later)
    token_id1 << t_id1.at(i);
    token_id2 << t_id2.at(i);//the actual id in the original formula (id1=graph-id; id2=set-id)
    token_pos << current_pos;//0=lhs, 1=rhs (we need to know this to be able to replace the ids with the correct new ids)
    token_control1 << t_control1.at(i);//control-character for Graph-ID ('$','#','N')
    token_control2 << t_control2.at(i);//control-character for Set-ID
    token_column << t_column.at(i);//[0,1,2,3,4]=[X,Y,Y1,Y2,Y3]
    }
}
}//for-loop of different commands

clear_replacements_and_counters();
}

void formula_to_process::add_one_token(int startpos,int type,int len,int id1,int id2,int pos,char control1,char control2,int col)
{
token_startpos << startpos;
token_type << type;
token_len << len;
token_id1 << id1;
token_id2 << id2;
token_pos << pos;
token_control1 << control1;
token_control2 << control2;
token_column << col;
}

void formula_to_process::return_qtspecial_subcommand(int nr,int r_type,int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f)
{
char * parameters;
    if (nr<0 || nr>=token_startpos.length() || token_type.at(nr)!=ID_SPECIAL_COMMAND)
    {
    f.clear();
    return;
    }
QString unchanged=original_formula.mid(token_startpos.at(nr),token_len.at(nr));
//qDebug() << "replacing qtSpecialCommand=" << unchanged << " control1=" << (int)token_control1.at(nr);
QString tmp_str;
//char tmp_cmd_str[512];
char * command=new char[8+2*token_len.at(nr)];
strcpy(command,unchanged.toLatin1().constData());
(void)containsSpecialCommand(command,&parameters);

int o_n_sets,n_sets,type,realization;
int * o_gnos=NULL,* o_snos=NULL;
int * gnos=NULL,* snos=NULL;
double limits[2];
int orders[2];
char x_formula[512];
double ripple;
int absolute,debug,point_extension,oversampling,rno,invr;
double start,stop;
int rx,ideg,iresid,ret;//,len2;
int pos,g_no,s_no,leng;
char control1,control2,col;
class formula_to_process * formula2;

    switch ((int)token_control1.at(nr))//type of special command
    {
    case SPECIAL_FILTER:
        if (r_type==2)
        {
ParseFilterCommand(command+28,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);
o_n_sets=1;
n_sets=1;
if (o_gnos==NULL) delete[] o_gnos;
if (o_snos==NULL) delete[] o_snos;
if (gnos==NULL) delete[] gnos;
if (snos==NULL) delete[] snos;
o_gnos=new int[2];
o_snos=new int[2];
gnos=new int[2];
snos=new int[2];
o_gnos[0]=g_rhs;
o_snos[0]=s_rhs;
gnos[0]=g_lhs;
snos[0]=s_lhs;
WriteFilterString(f,o_n_sets,o_gnos,o_snos,n_sets,gnos,snos,type,realization,limits,orders,x_formula,ripple,absolute,debug,point_extension,oversampling,rno,invr);
        }
        else
        {
        f=unchanged;
        }
        break;
    case SPECIAL_REGRESSION:
        if (r_type==2)
        {
ParseRegression(command+28,o_n_sets,&o_gnos,&o_snos,n_sets,&gnos,&snos,ideg,iresid,rno,invr,start,stop,point_extension,rx,x_formula);
o_n_sets=1;
n_sets=1;
if (o_gnos==NULL) delete[] o_gnos;
if (o_snos==NULL) delete[] o_snos;
if (gnos==NULL) delete[] gnos;
if (snos==NULL) delete[] snos;
o_gnos=new int[2];
o_snos=new int[2];
gnos=new int[2];
snos=new int[2];
o_gnos[0]=g_rhs;
o_snos[0]=s_rhs;
gnos[0]=g_lhs;
snos[0]=s_lhs;
WriteRegressionString(f,o_n_sets,o_gnos,o_snos,n_sets,gnos,snos,ideg,rno,invr,start,stop,point_extension,rx,x_formula);
        }
        else
        {
        f=unchanged;
        }
        break;
    case SPECIAL_APPEND:
        if (r_type==2 || r_type==1)//replaced or completed
        {
tmp_str=unchanged.mid(24);
ret=searchTextForFirstID(tmp_str,0,pos,g_no,s_no,control1,control2,col,leng);
//qDebug() << "Found in Append: G" << g_no << control1 << ".S" << s_no << control2;
tmp_str=unchanged.mid(24+pos+leng+1);
pos=tmp_str.lastIndexOf(QString("}"));
tmp_str=tmp_str.left(pos);
formula2=new formula_to_process;
formula2->init_formula(tmp_str);
formula2->sync_allocated_new_and_counter(this);
if (r_type==1)//complete id; original is already set up completely
formula2->return_completed_formula(g_rhs,s_rhs,g_rhs,s_rhs,tmp_str);
else if (r_type==2)
formula2->return_replaced_formula(g_rhs,s_rhs,g_rhs,s_rhs,tmp_str);
//qDebug() << "Append_value(original)=" << tmp_str;
        f=QString("#QTGRACE_SPECIAL APPEND G");
            if (control1=='N')//new graph
            {
            f+=QString::number(get_new_graph(g_no));
            }
            else if (control1=='#')//graph counter
            {
            f+=QString::number(get_graph_counter(g_no));
            }
            else if (control1=='$')//graph id unchanged
            {
            f+=QString::number(g_no);
            }
            else//normal graph-id
            {
                if (r_type==1)//complete = original here
                {
                f+=QString::number(g_no);
                }
                else
                {
                f+=QString::number(g_lhs);
                }
            }
        f+=QString(".S");
            if (control2=='N')//new set
            {
            f+=QString::number(get_new_set(g_no,control1,s_no));
            }
            else if (control2=='#')//set counter
            {
            f+=QString::number(get_set_counter(g_no,control1,s_no));
            }
            else if (control2=='$')//set id unchanged
            {
            f+=QString::number(s_no);
            }
            else//normal set-id
            {
                if (r_type==1)//complete = original here
                {
                f+=QString::number(s_no);
                }
                else
                {
                f+=QString::number(s_lhs);
                }
            }
        f+=QString("{");
//qDebug() << "Append_value(intended_replacement)=" << tmp_str << " f=" << f;
        f+=tmp_str;
        f+=QString("}");
//qDebug() << "f=" << f;
        this->sync_allocated_new_and_counter(formula2);
        delete formula2;
        }
        else//original
        {
        f=unchanged;
        }
        break;
    case SPECIAL_ADD:
    case SPECIAL_MINUS:
    case SPECIAL_DIVIDE:
    case SPECIAL_MULTIPLY:
        f=unchanged;
        /*if (strcmp(parameters,"LAST_VALUE")==0)
        {
            //sprintf(temp_val,"%g",saved_value);
        tmp_str=QString::number(saved_value);
        }
        else if (strcmp(parameters,"LAST_FORMULA")==0)
        {
            formula2=new formula_to_process;
            tmp_str=QString(last_formula);//original
            qDebug() << "last_formula=" << last_formula;
            formula2->init_formula(tmp_str);
            formula2->sync_allocated_new_and_counter(this);
            if (r_type==1)//complete
            {
            formula2->return_completed_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            else if (r_type==2)//replaced
            {
            formula2->return_replaced_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            strcpy(tmp_cmd_str,tmp_str.toLatin1().constData());
            len2=strlen(tmp_cmd_str);
            pos=0;
            for (int i=0;i<len2;i++){if (tmp_cmd_str[i]=='='){pos=i+1; break;}}
        //f+=QString(tmp_cmd_str+pos);
        tmp_str=QString(tmp_cmd_str).mid(pos);
            this->sync_allocated_new_and_counter(formula2);
            delete formula2;
        }
        else//SAVED_FORMULA
        {
            formula2=new formula_to_process;
            tmp_str=QString(saved_formula);//original
            formula2->init_formula(tmp_str);
            formula2->sync_allocated_new_and_counter(this);
            if (r_type==1)//complete
            {
            formula2->return_completed_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            else if (r_type==2)//replaced
            {
            formula2->return_replaced_formula(g_lhs,s_lhs,g_rhs,s_rhs,tmp_str);
            }
            strcpy(tmp_cmd_str,tmp_str.toLatin1().constData());
            len2=strlen(tmp_cmd_str);
            pos=0;
            for (int i=0;i<len2;i++){if (tmp_cmd_str[i]=='='){pos=i+1; break;}}
        //f+=QString(tmp_cmd_str+pos);
        tmp_str=QString(tmp_cmd_str).mid(pos);
            this->sync_allocated_new_and_counter(formula2);
            delete formula2;
        }

        if (r_type==0)
        {
        f=QString("Y=Y");
        }
        else
        {
        f=QString("G")+QString::number(g_lhs)+QString(".S")+QString::number(s_lhs)+QString(".Y=")+QString("G")+QString::number(g_lhs)+QString(".S")+QString::number(s_lhs)+QString(".Y");
        }

        if (token_control1.at(nr)==SPECIAL_ADD)
        {
        f+=QString("+(");
        }
        else if (token_control1.at(nr)==SPECIAL_MINUS)
        {
        f+=QString("-(");
        }
        else if (token_control1.at(nr)==SPECIAL_MULTIPLY)
        {
        f+=QString("*(");
        }
        else if (token_control1.at(nr)==SPECIAL_DIVIDE)
        {
        f+=QString("/(");
        }

        f+=tmp_str;

        f+=QString(")");*/
        break;
    case SPECIAL_FORMULA://no replacing in these cases
    case SPECIAL_REMEMBER:
    case SPECIAL_USE:
    case SPECIAL_NONE:
    case SPECIAL_EXTRACT:
    f=unchanged;
        break;
    }
//qDebug() << "Replaced Special Formula=" << f;
if (command!=NULL) delete[] command;
if (o_gnos==NULL) delete[] o_gnos;
if (o_snos==NULL) delete[] o_snos;
if (gnos==NULL) delete[] gnos;
if (snos==NULL) delete[] snos;
}

void formula_to_process::return_completed_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f)//returns a formula with completed ids (complete ids (like G0.S1.Y) stay untouched, incomplete ids (like just Y) get completed according to their position on the left hand side (lhs) or right hand side (rhs) of a '=')
{
static QString intermediate;
if (token_startpos.length()<1)
{
f=original_formula;
return;
}
f.clear();
for (int i=-1;i<token_startpos.length();i++)
{
    if (i>=0)
    {
    return_single_token(i,1,g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    f+=intermediate;
    }
get_string_after_token(i,intermediate);
f+=intermediate;
}
}

void formula_to_process::return_replaced_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & f)//completes incomplete ids and replaces existing ids
{
static QString intermediate;
if (token_startpos.length()<1)
{
f=original_formula;
return;
}
f.clear();
for (int i=-1;i<token_startpos.length();i++)
{
    if (i>=0)
    {
    //return_single_token(i,0,g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    //qDebug() << "Original=" << intermediate;
    return_single_token(i,2,g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    //qDebug() << "Replaced=" << intermediate;
    f+=intermediate;
    }
get_string_after_token(i,intermediate);
f+=intermediate;
//qDebug() << "return_replaced_formula: f=" << f;
}
}

void formula_to_process::check_destination_set_existence(int g_lhs,int s_lhs,int g_rhs,int s_rhs,int replace)
{
int cur_pos=-1;//location (left / right of '=')
int dest_g,dest_s,d_len,s_g,s_s,s_len;
QString ret_token;
char tokenID[128];
dest_g=dest_s=s_g=s_s=d_len=s_len=-1;
    for (int i=0;i<token_pos.length();i++)
    {
    return_single_token(i,replace,g_lhs,s_lhs,g_rhs,s_rhs,ret_token);
    strcpy(tokenID,ret_token.toUpper().toLatin1());
    //qDebug() << "tokenID=" << tokenID;
        if (cur_pos!=token_pos.at(i))
        {
        cur_pos=token_pos.at(i);
            if (cur_pos==0)
            {
            sscanf(tokenID,"G%d.S%d",&dest_g,&dest_s);
            }
            else
            {
            sscanf(tokenID,"G%d.S%d",&s_g,&s_s);
            }
        }
        if (dest_g>=0 && s_g>=0)
        {
        s_len=getsetlength(s_g,s_s);
        d_len=getsetlength(dest_g,dest_s);
        //qDebug() << "Destination=G" << dest_g << ".S" << dest_s << " Source=G" << s_g << ".S" << s_s << " slen=" << s_len << " dlen=" << d_len;
            if (s_len!=d_len && s_len>=0)
            {
            //(void)allocate_set(dest_g,dest_s);
            (void)copyset(s_g,s_s,dest_g,dest_s);
            }
        dest_g=dest_s=s_g=s_s=s_len=-1;
        }
    }
}

QString formula_to_process::execute_formula(int g_lhs,int s_lhs,int g_rhs,int s_rhs,int replace,int & result)
{
static QString intermediate;
int cur_p_gno,cur_p_sno;
cur_p_gno=get_parser_gno();
cur_p_sno=get_parser_setno();
set_parser_setno(g_lhs,s_lhs);
//qDebug() << "Current Set=" << g_lhs << " , " << s_lhs;
    if (replace==1)
    {
    return_replaced_formula(g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
//qDebug() << "A EXECUTE intermediate=" << intermediate << " [" << original_formula << "]";
    check_destination_set_existence(g_lhs,s_lhs,g_rhs,s_rhs,2);
    }
    else
    {
    return_completed_formula(g_lhs,s_lhs,g_rhs,s_rhs,intermediate);
    check_destination_set_existence(g_lhs,s_lhs,g_rhs,s_rhs,1);
    }
QStringList list2;
QList<int> offsets;
//qDebug() << "B EXECUTE intermediate=" << intermediate;
get_List_of_Subformulas(intermediate,list2,offsets);
char * formu=new char[8+intermediate.toLatin1().length()];
result=0;
bool old_active_replaceing=activate_id_replacing;
activate_id_replacing=false;
//qDebug() << "Current Set=G" << get_parser_gno() << ".S" << get_parser_setno();
for (int i=0;i<list2.length();i++)
{
//qDebug() << "EXECUTE subformula=" << list2.at(i);
strcpy(formu,list2.at(i).toLatin1().constData());
set_parser_setno(g_lhs,s_lhs);//for special replay-commands like LAST_FORMULA
result+=scanner(formu);
}
activate_id_replacing=old_active_replaceing;
increase_counters();
delete[] formu;
set_parser_setno(cur_p_gno,cur_p_sno);
return intermediate;
}

void formula_to_process::clear_replacements_and_counters(void)
{
    token_graph_new.clear();
    token_set_new.clear();
    token_graph_counter.clear();
    token_set_counter.clear();
    token_graph_counter_used.clear();
    token_set_counter_used.clear();
    for (int i=0;i<token_startpos.length();i++)
    {
    token_graph_new << -1;
    token_set_new << -1;
    token_graph_counter << token_id1.at(i);//initialize with first id
    token_set_counter << token_id2.at(i);//initialize with first id
    token_graph_counter_used << false;//only used ids will be increased later (usually all should get used)
    token_set_counter_used << false;
    }
}

void formula_to_process::increase_counters(void)
{
int id1, id2, value;
char control1, control2;
//first: search graph-counters
    for (int i=0;i<token_startpos.length();i++)
    {
        if (token_graph_counter_used.at(i)==true)//found a used counter
        {
        value=token_graph_counter.at(i)+1;//the increased value
        id1=token_id1.at(i);
        control1=token_control1.at(i);
            for (int j=i;j<token_startpos.length();j++)
            {
                if (token_id1.at(j)==id1 && token_control1.at(j)==control1)
                {
                token_graph_counter.replace(j,value);
                token_graph_counter_used.replace(j,false);
                }
            }
        }
    }
//second: search set-counters
    for (int i=0;i<token_startpos.length();i++)
    {
        if (token_set_counter_used.at(i)==true)//found a used counter
        {
        value=token_set_counter.at(i)+1;//the increased value
        id1=token_id1.at(i);
        control1=token_control1.at(i);
        id2=token_id2.at(i);
        control2=token_control2.at(i);
            for (int j=i;j<token_startpos.length();j++)
            {
                if (token_id1.at(j)==id1 && token_control1.at(j)==control1 && token_id2.at(j)==id2 && token_control2.at(j)==control2)
                {
                token_set_counter.replace(j,value);
                token_set_counter_used.replace(j,false);
                }
            }
        }
    }
}

int formula_to_process::get_graph_counter(int id1)
{
QList<int> index;
int value=0;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)=='#')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "G" << id1 << "# not found!";
    return -1;
}
else
{
    value=token_graph_counter.at(index.at(0));
    token_graph_counter_used.replace(index.at(0),true);
    //for (int i=0;i<index.length();i++) token_graph_counter.replace(index.at(i),value+1);//increase the counter
    return value;
}
return -1;
}

int formula_to_process::get_set_counter(int id1, char control1, int id2)
{
QList<int> index;
int value=0;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)==control1 && token_id2.at(i)==id2 && token_control2.at(i)=='#')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "S" << id2 << "# not found!";
    return -1;
}
else
{
    value=token_set_counter.at(index.at(0));
    token_set_counter_used.replace(index.at(0),true);//get value and say that the counter has been used
    //for (int i=0;i<index.length();i++) token_set_counter.replace(index.at(i),value+1);//increase the counter
    return value;
}
}

int formula_to_process::get_new_graph(int id1)
{
QList<int> index;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)=='N')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "G" << id1 << "N not found!";
    return -1;
}
else
{
    if (token_graph_new.at(index.at(0))==-1)//no new graph generated yet
    {
    /// create new graph, set id for all in index, return new graph-id
    token_graph_new.replace(index.at(0),create_new_graph());
        if (token_graph_new.at(index.at(0))!=-1)
        {
            for (int i=1;i<index.length();i++)
            {
            token_graph_new.replace(index.at(i),token_graph_new.at(index.at(0)));
            }
        return token_graph_new.at(index.at(0));
        }
    }
    else//new graph already created
    {
    return token_graph_new.at(index.at(0));
    }
}
return -1;
}

int formula_to_process::get_new_set(int id1, char control1, int id2)
{
QList<int> index;
index.clear();
for (int i=0;i<token_startpos.length();i++)
{
    if (token_id1.at(i)==id1 && token_control1.at(i)==control1 && token_id2.at(i)==id2 && token_control2.at(i)=='N')
    {
    index << i;
    }
}
if (index.length()<1)
{
    qDebug() << "S" << id2 << "N not found!";
    return -1;
}
else
{
    if (token_set_new.at(index.at(0))==-1)//no new graph generated yet
    {
    /// create new graph, set id for all in index, return new graph-id
    token_set_new.replace(index.at(0),nextset(get_current_graph_id(index.at(0))));
        if (token_set_new.at(index.at(0))!=-1)
        {
            for (int i=1;i<index.length();i++)
            {
            token_set_new.replace(index.at(i),token_set_new.at(index.at(0)));
            }
        return token_set_new.at(index.at(0));
        }
    }
    else//new graph already created
    {
    return token_set_new.at(index.at(0));
    }
}
return -1;
}

int formula_to_process::get_current_graph_id(int nr)
{
int ret=-1;
    switch (token_type.at(nr))
    {
    case ID_REGULAR_COMPLETE:
    case ID_SGNS:
    case ID_GRAPH_ONLY:
        ret=token_id1.at(nr);
    break;
    case ID_NGRAPH_ONLY:
    case ID_NGSS:
    case ID_NGNS:
        if (token_control1.at(nr)=='N')
        ret=token_graph_new.at(nr);
        else if (token_control1.at(nr)=='$')
        ret=token_id1.at(nr);
        else
        ret=token_graph_counter.at(nr);
    break;
    case ID_SET_ONLY:
    case ID_NSET_ONLY:
    case ID_COLUMN_ONLY:
    case ID_GRAPH_NR_ONLY:
        ret=current_graph;
    break;
    case ID_SET_NR_ONLY:
        ret=-1;
    break;
    }
return ret;
}

int formula_to_process::get_current_set_id(int nr)
{
int ret=-1;
    switch (token_type.at(nr))
    {
    case ID_REGULAR_COMPLETE:
    case ID_NGSS:
    case ID_SET_ONLY:
        ret=token_id2.at(nr);
    break;
    case ID_SGNS:
    case ID_NGNS:
    case ID_NSET_ONLY:
        if (token_control2.at(nr)=='N')
        ret=token_set_new.at(nr);
        else if (token_control2.at(nr)=='$')
        ret=token_id2.at(nr);
        else
        ret=token_set_counter.at(nr);
    break;
    case ID_GRAPH_ONLY:
    case ID_GRAPH_NR_ONLY:
    case ID_NGRAPH_ONLY:
        ret=-1;
    break;
    case ID_COLUMN_ONLY:
    case ID_SET_NR_ONLY:
        ret=current_set;
    break;
    }
return ret;
}

void formula_to_process::clear_tokens(void)
{
token_startpos.clear();
token_type.clear();
token_len.clear();
token_id1.clear();
token_id2.clear();
token_pos.clear();
token_control1.clear();
token_control2.clear();
token_column.clear();
token_graph_new.clear();
token_set_new.clear();
token_graph_counter.clear();
token_set_counter.clear();
token_graph_counter_used.clear();
token_set_counter_used.clear();
}

void formula_to_process::return_single_token(int nr,int type,int g_lhs,int s_lhs,int g_rhs,int s_rhs,QString & new_token)
{
//type=0 --> original | type=1 --> completed if incomplete | type=2 --> replaced
static const char * cols[5]={"X","Y","Y1","Y2","Y3"};
new_token.clear();
if (nr<0||nr>=token_startpos.length())
{
qDebug() << "Not a valid token nr=" << nr << " token-count=" << token_startpos.length();
return;
}
int gno=0,sno=0;
if (token_pos.at(nr)==0)//lhs
{
gno=g_lhs;
sno=s_lhs;
}
else//rhs
{
gno=g_rhs;
sno=s_rhs;
}
current_graph=gno;
current_set=sno;
//qDebug() << "Nr=" << nr << " Type=" << token_type.at(nr);
switch (token_type.at(nr))
{
case ID_NONE:
qDebug() << "No Id here!?";
break;
case ID_REGULAR_COMPLETE:
    if (type==0 || type==1)//original and completed are the same, because original is complete
    new_token=QString("G")+QString::number(token_id1.at(nr))+QString(".S")+QString::number(token_id2.at(nr));
    else
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_NGSS:
    //check whether the new graph already contains the Set-Id-mentioned
    /*if (!is_valid_setno(get_new_graph(token_id1.at(nr)),token_id2.at(nr)))
    {
    //qDebug() << "SET DOES NOT(!) EXIST: G" << get_new_graph(token_id1.at(nr)) << ".S" << token_id2.at(nr);
    (void)allocate_set(get_new_graph(token_id1.at(nr)),token_id2.at(nr));
    }
    else
    {
    qDebug() << "SET DOES EXIST: G" << get_new_graph(token_id1.at(nr)) << ".S" << token_id2.at(nr);
    }*/
case ID_SGNS:
case ID_NGNS:
    new_token=QString("G");
    if (token_type.at(nr)==ID_SGNS)
    {//standard-graph-id
        if (type==0 || type==1)//original or completed
        {
        new_token+=QString::number(token_id1.at(nr));
        }
        else//replaced
        {
        new_token+=QString::number(gno);
        }
    }
    else
    {//new-graph-id / special-id with control character
        if (type==0)//original
        {
        new_token+=QString::number(token_id1.at(nr))+QString(token_control1.at(nr));
        }
        else//completed or replaced (special ids are not to be replaced)
        {
            if (token_control1.at(nr)=='N')
            {
            new_token+=QString::number(get_new_graph(token_id1.at(nr)));
            }
            else if (token_control1.at(nr)=='#')
            {
            new_token+=QString::number(get_graph_counter(token_id1.at(nr)));
            }
            else if (token_control1.at(nr)=='$')
            {
            new_token+=QString::number(token_id1.at(nr));//unchanged
            }
        }
    }
    new_token+=QString(".S");
    if (token_type.at(nr)==ID_NGSS)
    {//standard-set-id
        if (type==0 || type==1)//original or completed
        {
        new_token+=QString::number(token_id2.at(nr));
        }
        else//replaced
        {
        new_token+=QString::number(sno);
        }
    }
    else
    {//new-set-id / special-id with control character
        if (type==0)//original
        {
        new_token+=QString::number(token_id1.at(nr))+QString(token_control1.at(nr));
        }
        else//completed or replaced are the same here
        {
            if (token_control2.at(nr)=='N')
            {
            new_token+=QString::number(get_new_set(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
            }
            else if (token_control2.at(nr)=='#')
            {
            new_token+=QString::number(get_set_counter(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
            }
            else if (token_control2.at(nr)=='$')
            {
            new_token+=QString::number(token_id2.at(nr));//unchanged
            }
        }
    }
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_SET_ONLY:
    if (type==0)//original
    new_token=QString("S")+QString::number(token_id2.at(nr));
    else if (type==1)//completed
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(token_id2.at(nr));
    else//replaced
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno);
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_NSET_ONLY:
    if (type==0)//original
    {
    new_token=QString("S")+QString::number(token_id2.at(nr))+QString(token_control2.at(nr));
    }
    else//completed or replaced (i.e. resolve control character)
    {
    new_token=QString("G")+QString::number(gno)+QString(".S");//the original id did not contain a graph-number
        if (token_control2.at(nr)=='$')//unchanged id
        {
        new_token+=QString::number(token_id2.at(nr));//return the id unchanged
        }
        else if (token_control2.at(nr)=='#')//id-counter
        {
        new_token+=QString::number(get_set_counter(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
        }
        else if (token_control2.at(nr)=='N')//new id
        {
        new_token+=QString::number(get_new_set(token_id1.at(nr),token_control1.at(nr),token_id2.at(nr)));
        }
    }
    if (token_column.at(nr)>=0 && token_column.at(nr)<5)//if needed add a X or Y or something
    new_token+=QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_GRAPH_ONLY:
    if (type==0 || type==1)//original or completed
    new_token=QString("G")+QString::number(token_id1.at(nr));
    else//replaced
    new_token=QString("G")+QString::number(gno);
break;
case ID_NGRAPH_ONLY:
    if (type==0)//original
    {
    new_token=QString("G")+QString::number(token_id1.at(nr))+QString(token_control1.at(nr));
    }
    else//completed or replaced (i.e. resolve control character)
    {
        if (token_control1.at(nr)=='$')//unchanged id
        {
        new_token=QString("G")+QString::number(token_id1.at(nr));//return the id unchanged
        }
        else if (token_control1.at(nr)=='#')//id-counter
        {
        new_token=QString("G")+QString::number(get_graph_counter(token_id1.at(nr)));
        }
        else if (token_control1.at(nr)=='N')//new id
        {
        new_token=QString("G")+QString::number(get_new_graph(token_id1.at(nr)));
        }
    }
break;
case ID_COLUMN_ONLY:
    if (type==0)//original
    new_token=QString(cols[token_column.at(nr)]);
    else//completed or replaced (is the same, because no ids have been given at all)
    new_token=QString("G")+QString::number(gno)+QString(".S")+QString::number(sno)+QString(".")+QString(cols[token_column.at(nr)]);
break;
case ID_GRAPH_NR_ONLY:
    if (type==0)
    new_token=QString("G#");
    else//completed or replaced --> just return current graph-nr
    new_token=QString::number(gno);
break;
case ID_SET_NR_ONLY:
    if (type==0)
    new_token=QString("S#");
    else//completed or replaced --> just return current set-nr
    new_token=QString::number(sno);
break;
case ID_SPECIAL_COMMAND:
    return_qtspecial_subcommand(nr,type,g_lhs,s_lhs,g_rhs,s_rhs,new_token);
break;
}
}

void formula_to_process::get_List_of_Subformulas(QString text,QStringList & subtext,QList<int> & offsets)
{
subtext.clear();
offsets.clear();
int offset=0;
int citation_counter=0;
int brakets_counter=0;
char c;
for (int i=0;i<text.length();i++)
{
c=text.at(i).toLatin1();
//qDebug() << c;
    if (c=='\"')
    {
    citation_counter=!citation_counter;
    }
    if (c=='{') brakets_counter++;
    if (c=='}') brakets_counter--;
    if ((c==';' && citation_counter==0 && brakets_counter==0) || i==text.length()-1)//end of a sub-formula
    {
        if (i==text.length()-1)
        subtext << text.mid(offset,i-offset+1);
        else
        subtext << text.mid(offset,i-offset);
    offsets << offset;
    offset=i+1;
    //qDebug() << "SAVED, OFFSET=" << offsets.last();
    }
}
}

void formula_to_process::get_string_after_token(int nr,QString & text)
{
    if (token_startpos.length()<1)//no tokens found --> return everything
    {
    text=original_formula;
    return;
    }
    if (nr==-1)//return the beginning
    {
    text=original_formula.left(token_startpos.at(0));
    }
    else if (nr>=token_startpos.length()-1)//text after last token
    {
    text=original_formula.right(original_formula.length()-(token_startpos.last()+token_len.last()));
    }
    else//some token inbetween
    {
    text=original_formula.mid(token_startpos.at(nr)+token_len.at(nr),token_startpos.at(nr+1)-(token_startpos.at(nr)+token_len.at(nr)));
    }
}

void formula_to_process::sync_allocated_new_and_counter(class formula_to_process * other_formula)
{
int found_id;
//we have to find an id in this formula, that has the same type as in the other one and sync the replacements ids (this is only useful for new ids or counters)
for (int i=0;i<token_startpos.length();i++)
{
    if (token_control1.at(i)=='#' || token_control1.at(i)=='N' || token_control2.at(i)=='#' || token_control2.at(i)=='N')//the current id is actually a counter or something new (all others are irrelevant here)
    {
    found_id=-1;
        for (int j=0;j<other_formula->token_startpos.length();j++)
        {//go through all other ids and find a similar one
            if (other_formula->token_type.at(j)==token_type.at(i) &&
                other_formula->token_id1.at(j)==token_id1.at(i) &&
                other_formula->token_control1.at(j)==token_control1.at(i) &&
                other_formula->token_id2.at(j)==token_id2.at(i) &&
                other_formula->token_control2.at(j)==token_control2.at(i) )
            {
            found_id=j;
            break;
            }
        }
        //if one suitable id is found: copy entries
        if (found_id>=0)
        {
        token_graph_new.replace(i,other_formula->token_graph_new.at(found_id));
        token_set_new.replace(i,other_formula->token_set_new.at(found_id));
        token_graph_counter.replace(i,other_formula->token_graph_counter.at(found_id));
        token_set_counter.replace(i,other_formula->token_set_counter.at(found_id));
        token_graph_counter_used.replace(i,other_formula->token_graph_counter_used.at(found_id));
        token_set_counter_used.replace(i,other_formula->token_set_counter_used.at(found_id));
        }
    }
}
}


