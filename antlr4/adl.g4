grammar adl;

process: NIL                                                #nil
       | NOT? LOWERCASE DOT process                         #action
       | process PLS process                                #alternative
       | process PAR process                                #parallel
       | LPR NUL RBR lowercase_com LBR process              #restriction
       | LPR process RPR                                    #parprocess
       | ULOWER DEF process INN process                      #letinprocess
       ;

naming: LOWERCASE                                           #simple
      | ULOWER DOT LOWERCASE                                #processComponent
      ;

lowercase_com: (LOWERCASE COM)+ LOWERCASE;
composite_com: (naming COM)+ naming;

component: 'component' ULOWER DEF 'ports' component_port+ ;
connector: 'connector' ULOWER DEF 'roles' component_port+ ;
component_port : port_or_role=LOWERCASE LPR lowercase_com RPR DEF process;

adl: 'infrastructure' ULOWER
     'components' DEF component+ DOT
     'connectors' DEF connector+ DOT
     'where' adl_commands;

adl_commands: ULOWER 'is-active'                            #active_component
            | ULOWER 'on-standby'                           #standby_component
            | ULOWER DOT LOWERCASE LPR composite_com? RPR   #establish_link
            | 'break' naming 'to' naming                    #unlink
            | 'init' hml_system                             #initials
            | 'fini' hml_system                             #finals
            ;

hml_system: 'system' hml_type hml*
   ;

hml: hml AND hml            #and
   | hml LOR  hml            #or
   | REC INTNUMBER          #rec
   | BOX LOWERCASE DOT hml  #box
   | DIA LOWERCASE DOT hml  #dia
   | TRU                    #true
   | FAL                    #false
   ;

hml_type : MIN #min
         | MAX #max
         ;

MIN: 'MIN';
MAX: 'MAX';
REC: 'REC';
TRU: 'true';
FAL: 'false';
BOX: '□';
DIA: '◇';
NIL: '0';
NOT: '~';
DOT: '.';
PLS: '+';
PAR: '|';
NUL: 'ν';
LPR: '(';
RPR: ')';
RBR: '{';
LBR: '}';
COM: ',';
DEF: ':=';
INN: 'in';
AND: '∧';
LOR: '∨';
INTNUMBER : ('0'..'9')+ ;
LOWERCASE: [a-z]+;
ULOWER: [A-Z][a-z]+;

SPACE : [ \t\r\n]+ -> skip;

COMMENT
    : '/*' .*? '*/' -> skip
;

LINE_COMMENT
    : '//' ~[\r\n]* -> skip
;