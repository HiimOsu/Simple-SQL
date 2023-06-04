#include "gtest/gtest.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "../../includes/bplustree/map.h"
#include "../../includes/bplustree/multimap.h"
#include "../../includes/parser/parser.h"
#include "../../includes/table/table.h"
#include "../../includes/sql/sql.h"
using namespace std;

typedef Map<string, int> map_si;

Table make_table(){
  Table t("student", {"fname", "lname","age"});
  
  vectorstr row;
  row = {"Joe", "Gomez", "20"};
  t.insert_into(row);
  row = {"Karen", "Orozco", "21"};
  t.insert_into(row);
  row = {"Flo", "Yao", "29"};
  t.insert_into(row);
  row = {"Jack", "Yao", "19"};
  t.insert_into(row);
  row = {"Flo", "Jackson", "20"};
  t.insert_into(row);
  row = {"Flo", "Gomez", "20"};
  t.insert_into(row);
  
  return t;
}


TEST(VMAP, DISABLED_vector_of_map) {
  vector<map_si> si_map;
  int size = 5;
  si_map.reserve(size);
  for(int i =0; i < size;++i){
    si_map.push_back(map_si());
  }

  for(auto w = si_map.begin(); w != si_map.end(); ++w){
    cout << "Aye\n";
  }

  si_map[size-1].insert("age", 0); 
  cout << si_map[size-1] << endl;
  cout << si_map[size-1].at("age") << endl;
  cout << si_map[size-1].get("age") << endl;
  cout << "\n=====================\n";
  si_map[size-1].at("age") +=2;
  si_map[size-1].get("age") +=3;

  cout << si_map[size-1] << endl;
  cout << si_map[size-1].at("age") << endl;
  cout << si_map[size-1].get("age") << endl;
  cout << "\n=====================\n";

  si_map[size-1].at("non_existant1");
  si_map[size-1].get("non_existant2");

  cout << "\n\nsi_map[size-1]:\n" << si_map[size-1] << endl;

  si_map[0] = si_map[size-1];

  cout << "\n\nsi_map[0]:\n" << si_map[0] << endl;
  
}

TEST(TABLE, DISABLED_make_table){
  Table t = make_table();
  
  cout << "\n\n\n"
    << "Make Table, T =\n"
    << t << "\n\n\n";

  // t.debug_print();
  
}


TEST(PARSER, DISABLED_parser){
  vectorstr command = {
    /*0*/   "MAKE table student fields abc defg hijklmn opq rst",
    /*1*/   "MAKE table student fields abc, defg, hijklmn, opq, rst",
    /*2*/   "MAKE table student fields abc defg hijklmn, \topq \t,rst \n",
    /*3*/   "SELECT * from student",
    /*4*/   "SELECT abc defg hijklmn opq rst from student",
    /*5*/   "SELECT abc, defg, hijklmn,opr,rst from student",
    /*6*/   "SELECT abc defg hijklmn\topq ,rst from student",

        //============Too long===============/
    /*7*/   "SELECT 111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111 from \"too_long\"",
    /*8*/   "SELECT 11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112222 from \"too_long\"",
    /*9*/   "SELECT 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111,2222 from \"too_long\"",
    /*10*/   "SELECT 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111 2222 from \"too_long\"",
    /*11*/   "SELECT 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111 2222 from \"too_long\"",
    /*12*/   "SELECT \"1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111112222\" from \"too_long\"",
    /*13*/   "SELECT \"111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\"2222 from \"too_long\"",
    /*14*/   "SELECT 1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111\"222_3\" from \"too_long\"",

  };
  Parser p;
  for(int i = 0; i < 15; ++i){
    p.set_command(command[i]);
    
    p.parse_tree().print_lookup();
    cout << "\n\n===========================================\n\n";
  }

}

vectorstr make_cmd1= {
  "Make table student fields  id      lname   fname     major   dob",
  "Insert into student values 1514,   Lee,  ChungYin,  CS,    \"02Sep\"",
  "Insert into student values 2090,   Aee,  AungAin,   Aath,   \"02May\"",
  "Insert into student values 3030,   Bee,  BungBin,   Bath,   \"23June\"",
  "Insert into student values 4321,   Cee,  CungCin,   Cath,  \"05August\"",
  "Insert into student values 5042,   Dee,  DungDin,   Dath,   \"05Dec\"",
  "Insert into student values 6397,   Wee,  WungWin,   PublicHealth,   \"13Dec\",  ",
  "Insert into student values 7777,   Lee,  Lunglin,   LeatherMaking,   \"13Dec",
  "Insert into student values 9991,   0Inserting, 0Extra , DataScience, 20April, Extra1, Extra2, Extra3",
  "Insert into student values 9992,   0Inserting, 0Lesser, Physics",
  "Insert into student values 9993,                               ",
  
//Harder
  "abc into student ...", //unknown cmd
//(insert)
  "Insert into employee values 1, 2, 3", //Table does not exist
  "Insert abadasd", //missing into(key word)
  "insert into stduent adasdas asda", //missing values(key word)
  "insert into stduent values", //missing values 
//(Make)
  "Make abc abc fields 1 2 3", // missing table(key word)
  "Make table fields 1 2 3", // missing table_name
  "Make table abc abc 1 2 3", // missing fields(key word)
  "Make table abc fields", // missing fields
};
vectorstr select_cmd1 = {
  /*0*/ "Select * from student",
  /*1*/ "Select id lname major from student",
  /*2*/ "Select fname dob non-exist from student",
  /*3*/ "Select fname lname dob from student where lname = Lee OR dob >= 13",
  /*4*/ "Select fname from student where fname > 0",
  /*5*/ "Select fname from student where fname < D and fname >= A",

  //Harder
  /*6*/  "Select fname from student where fname < D and fname >= A",
  /*7*/  "Select fname from student where fname < D and fname >= A",
  /*8*/  "Select fname, major from student where (fname < D and fname >= A) OR (major = DataScience)",
  /*9*/  "Select dob, major from student where (dob <= 10 OR dob >= 20) and (major = DataScience or major >= C)",
  /*10*/  "Select dne, dob, major  from student where (dob <= 10 OR dob >= 20) and (major = DataScience or major >= D)",
  /*10*/  "Select dne, dob, major  from student where (dob <= 10 OR dob >= 20)",

  //Even harder
  /*11*/  "Select fname from student where (dob <= 10 OR dob >= 20) and (major = DataScience or major >= D",
  /*11*/  "Select fname from student where (dob <= 10 OR dob >= 20) and ",
};
TEST(PARSER, DISABLED_parser2_make){
  Parser p;
  for(auto x : make_cmd1){
    p.set_command(x);
    
    p.parse_tree().print_lookup();
    cout << "\n\n===========================================\n\n";
  }
}
TEST(PARSER, DISABLED_parser2_select){
  Parser p;
  for(auto x : select_cmd1){
    p.set_command(x);
    
    p.parse_tree().print_lookup();
    cout << "\n\n===========================================\n\n";
  }
}

TEST(SQL, test1){
  SQL sql;
  Table selected;
  fstream f;
  if(!f.is_open()){
    cout << "F is not open initially\n";
  }
  bool debug = false;


  for(auto x : make_cmd1){
    cout << "\ncommand > \"" << x << "\"\n"; 
    selected = sql.command(x);
    if(debug){
      cout << endl << selected << endl;
      cout << "selected #no: " << sql.select_recnos() << "\n\n";
    }
  }

  cout << "\n\nTotal commands: " << make_cmd1.size() << endl;
  cout << "================================\n\n";
  

  cout << "\t\t\tSELECT:\n\n\n";

  for(auto x : select_cmd1){
    cout << "\n----------------------------------------------------------------------------\n"
          << "command > \"" << x << "\"\n"; 
    selected = sql.command(x);

    if(debug){
      cout << endl << selected << endl;
    }
    cout << "selected records: " << sql.select_recnos() << endl;
  }

}


vectorstr batch = {
  //****************************************************************************
//		CREATE AND INSERT
//****************************************************************************

//---- employee table ----------
/*1*/"make table employee fields  last, 		first, 			dep",
/*2*/"insert into employee values Blow, 		Joe, 			CS",
/*3*/"insert into employee values Johnson, 	\"Jimmy\", 		Chemistry",
/*4*/"insert into employee values Yang, 		Bo, 			CS",
/*5*/"insert into employee values \"Jackson\",	Billy, 			Math",
/*6*/"insert into employee values Johnson,	Billy, 			\"Phys Ed\"",
/*7*/"insert into employee values \"Van Gogh\",	\"Jim Bob\", 		\"Phys Ed\"",

/*8*/"select * from employee ",

//----- student table ----------
/*9*/"make table student fields 	fname, 			lname, 		major, 				age",
/*10*/"insert into student values 	Flo, 			Yao, 		CS, 				20",
/*11*/"insert into student values 	\"Flo\", 			\"Jackson\", 	Math,	 			21",
/*12*/"insert into student values 	Calvin, 		Woo, 		Physics,			22",
/*13*/"insert into student values 	\"Anna Grace\", 	\"Del Rio\", 	CS,	 				22",
/*14*/"select * from student",



//****************************************************************************
//		SIMPLE SELECT 
//****************************************************************************

"select * from student",

//------- simple strings -------------------
"select * from student where lname = Jackson",

//----- quoted strings ---------------------
"select * from student where lname = \"Del Rio\"",

//-------- non-existing string ------------------
"select * from student where lname = \"Does Not Exist\"",

//****************************************************************************
//		RELATIONAL OPERATORS:
//****************************************************************************

//.................
//:Greater Than   :
//.................
"select * from student where lname > Yang",


//. . . . . . . . . . . . . (Greater Than: Non-existing) . . . . . . . . . . . . . . . . . . . . . 
"select * from student where age > 50",

//. . . . . . . . . . . . . (Greater than: last item) . . . . . . . . . . . . . . . . . .
"select * from student where age  > 53",

//. . . . . . . . . . . . . (Greater Than: Passed last item) . . . . . . . . . . . . . . . . . . . . . 
"select * from student where age > 54",

//.................
//:Greater Equal  :
//.................
"select * from student where lname >= Yang",
//. . . . . .  (Greater Equal non-existing: ) . . . . . . . . . . . 
"select * from employee where last >= Jack",


//.................
//:Less Than      :
//.................


//. . . . . . . . . . . . . (Less Than: Non-existing) . . . . . . . . . . . . . . . . . . . . . 
"select * from student where lname < Jackson",

//. . . . . . . . . . . . . (Less than: first item) . . . . . . . . . . . . . . . . . .
"select * from student where age  < 20",


//. . . . . . . . . . . . . (Less Than: before first item) . . . . . . . . . . . . . . . . . . . . . 

"select * from student where age  < 19",


//.................
//:Less Equal     :
//.................

"select * from student where lname <= Smith",

//. . . . . .  (Less Equal non-existing: ) . . . . . . . . . . . 
"select * from employee where last <= Peach",

//****************************************************************************
//		LOGICAL OPERATORS
//****************************************************************************


//.................
//:AND            :
//.................

"select * from student where fname = \"Flo\" and lname = \"Yao\"",


//.................
//:OR            :
//.................
"select * from student where fname = Flo or lname = Jackson",


//.................
//:OR AND         :
//.................
"select * from student where fname = Flo or major = CS and age <= 23",


//.................
//:AND OR AND     :
//.................

"select * from student where age <30 and major=CS or major = Physics and lname = Jackson",


//.................
//:OR AND OR      :
//.................

"select * from student where lname = Yang or major = CS and age < 23 or lname = Jackson ",


}
;

TEST(SQL, DISABLED_batch){
  SQL sql;
  Table selected;
  int i = 0;
  for(auto x : batch){
    cout << "\n#" << i << "  command > \"" << x << "\"\n"; 
    selected = sql.command(x);

    if(i >=13 || i == 7){
      cout << selected << endl;
      cout << "selected #no: " << sql.select_recnos() << "\n\n";
    }

    ++i;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running testB.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}

