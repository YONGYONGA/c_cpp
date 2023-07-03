#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "mysql.h"
#include<iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>
#include <fstream>
using namespace std;
#pragma comment(lib, "libmysql.lib")

const char* host = "localhost";
const char* user = "root";
const char* pw = "mysql";
const char* db = "project";
char temp[10000];
int print() {
	printf("------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE 1\n");
	printf("\t2. TYPE 2\n");
	printf("\t3. TYPE 3\n");
	printf("\t4. TYPE 4\n");
	printf("\t5. TYPE 5\n");
	printf("\t0. QUIT\n");
	int a;
	scanf("%d", &a);
	return a;

	//printf("----------------------------------\n");
	printf("\n\n");
	printf("---- TYPE 5 ----\n\n");
	printf("** Find the top k brands by unit sales by the year**\n");
	printf(" Which K? : 3\n");
}
//datetime1이 더 크면. 그니깐. 더 늦거나 같은 시간일시. true
bool compareDatetime(string datetime1,string datetime2) {
	int year1 = std::stoi(datetime1.substr(0, 4));
	int month1 = std::stoi(datetime1.substr(5, 2));
	int day1 = std::stoi(datetime1.substr(8, 2));
	int hour1 = std::stoi(datetime1.substr(11, 2));
	int minute1 = std::stoi(datetime1.substr(14, 2));
	int second1 = std::stoi(datetime1.substr(17, 2));

	int year2 = std::stoi(datetime2.substr(0, 4));
	int month2 = std::stoi(datetime2.substr(5, 2));
	int day2 = std::stoi(datetime2.substr(8, 2));
	int hour2 = std::stoi(datetime2.substr(11, 2));
	int minute2 = std::stoi(datetime2.substr(14, 2));
	int second2 = std::stoi(datetime2.substr(17, 2));

	if (year1 > year2)
		return true;
	else if (year1 < year2)
		return false;
	else {
		if (month1 > month2)
			return true;
		else if (month1 < month2)
			return false;
		else {
			if (day1 > day2)
				return true;
			else if (day1 < day2)
				return false;
			else {
				if (hour1 > hour2)
					return true;
				else if (hour1 < hour2)
					return false;
				else {
					if (minute1 > minute2)
						return true;
					else if (minute1 < minute2)
						return false;
					else {
						if (second1 >= second2)
							return true;
						else
							return false;
					}
				}
			}
		}
	}
}
int main(void) {

	MYSQL* connection = NULL;
	MYSQL conn;
	MYSQL_RES* sql_result;
	MYSQL_ROW sql_row;
	vector<string> car_waybill;
	vector<pair<string, string>> arrival_time;
	
	mysql_init(&conn);
	if (!mysql_real_connect(&conn, host, user, pw, db, 0, NULL, 0)) {
		cerr << "MySQL 연결 실패: " << mysql_error(&conn) << endl;
		return 1;
	}

	string filename = "create.sql";
	ifstream file(filename);

	if (!file.is_open()) {
		cerr << "파일 열기 실패" << endl;
		return 1;
	}

	string line;
	string query;
	while (getline(file, line)) {
		query += line;
		// 쿼리 종결자인 ';'를 기준으로 한 쿼리씩 분리하여 실행
		if (!line.empty() && line.back() == ';') {
			if (mysql_query(&conn, query.c_str()) != 0) {
				cerr << "쿼리 실행 실패: " << mysql_error(&conn) << endl;
				return 1;
			}
			query.clear();
		}
	}

	file.close();
	mysql_close(&conn);





	//insert파일 읽기

	mysql_init(&conn);

	if (!mysql_real_connect(&conn, host, user, pw, db, 0, NULL, 0))
	{
		cerr << "MySQL 연결 실패: " << mysql_error(&conn) << endl;
		return 1;
	}

	filename = "insert.sql";
	ifstream files(filename);

	if (!files.is_open()) {
		cerr << "파일 열기 실패" << endl;
		return 1;
	}

	//string line;
	int count = 0;
	while (getline(files, line)) {
		if (line == "")
			continue;
		if (mysql_query(&conn, line.c_str()) != 0) {
			cerr << count<<" : 쿼리 실행 실패: " << mysql_error(&conn) << endl;
			return 1;
		}
	}

	files.close();
	mysql_close(&conn);

	//insert종료.

	if (mysql_init(&conn) == NULL)
		printf("mysql_init() error!");

	connection = mysql_real_connect(&conn, host, user, pw, db, 3306, (const char*)NULL, 0);
	if (connection == NULL)
	{
		printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
		return 1;
	}
	else
	{
		printf("Connection Succeed\n");
		int state = 0;




		if (mysql_select_db(&conn, db))
		{
			printf("%d ERROR : %s\n", mysql_errno(&conn), mysql_error(&conn));
			return 1;
		}//sql연동코드

	
		int option;
		int option2;
		while (1) {
			string query = "";
			string year = "";
			string time = "";
			car_waybill.clear();
			option = print();
			int count=0;
			if (option == 0)
				break;
			if (option == 1) {
				string car_number;
				string base_date;
				printf("\n\n");
				printf("---- TYPE 1 ----\n\n");
				printf("**  Assume truck X is destroyed in a crash.**\n");
				printf(" Which truck? : ");
				cin >> car_number;
				query = "SELECT * FROM driver WHERE car_number = "+car_number;
				int state = 0;
				state = mysql_query(connection, query.c_str());

				if (mysql_query(connection, query.c_str()) == 1) {
					sql_result = mysql_store_result(connection);
					if ((sql_row = mysql_fetch_row(sql_result)) == NULL) {
						cout << "no car\n";
					}
					else {
						mysql_free_result(sql_result);
						while(1)
						{
							printf("\n\n");
							printf("---- Subtypes in TYPE 1 ----\n\n");
							printf("	1. TYPE 1.1.\n");
							printf("	1. TYPE 1.2.\n");
							printf("	1. TYPE 1.3.\n");
							cin >> option2;
							if (option2 == 0) {
								cout << "input 0, back to main menu\n";
								break;
							}
							printf("input car destroyed date (yyyy-mm-dd hh:mm:ss) : ");
							cin >> year;
							cin >> time;
							year = year + " " + time;
							query = "SELECT deliver_assgn.waybill_number, deliver_assgn.depart_time, deliver_assgn.destination, hub.arrival_time, receiver.arrival_data, driver.car_number, ready_deliver.c_ID,receiver.name "
								"From deliver_assgn LEFT OUTER JOIN hub ON deliver_assgn.waybill_number =hub.waybill_number AND deliver_assgn.destination=hub.address "
								"LEFT OUTER JOIN receiver ON deliver_assgn.waybill_number = receiver.waybill_number AND deliver_assgn.destination=receiver.address "
								"LEFT OUTER JOIN driver ON deliver_assgn.d_id =driver.id "
								"LEFT OUTER JOIN ready_deliver ON deliver_assgn.waybill_number =ready_deliver.waybill_number "
								"ORDER BY deliver_assgn.depart_time";
							int state = 0;

							state = mysql_query(connection, query.c_str());
							if (state == 0)
							{
								cout << " result is:  \n";

								sql_result = mysql_store_result(connection);

								while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
								{
									if (sql_row[5] == car_number) {
										//그 차를 거쳐간 모든 운송장 번호들 벡터
										car_waybill.push_back(sql_row[0]);
										if (sql_row[3]) {
											arrival_time.push_back({ sql_row[3],sql_row[0] });
										}
										else if (sql_row[4]) {
											arrival_time.push_back({ sql_row[4],sql_row[0] });
										}
									}
									//printf("%s's %s %s  %s %s %s)\n", sql_row[5], sql_row[0], sql_row[1], sql_row[2], sql_row[3], sql_row[4]);
									//count++;

								}
								//다시 탐색.
								mysql_free_result(sql_result);
								int state = 0;
								state = mysql_query(connection, query.c_str());
								sql_result = mysql_store_result(connection);

								if (option2 == 1) {
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										if(find(car_waybill.begin(),car_waybill.end(),sql_row[0])!=car_waybill.end())
										{
											//hub에게
											if (sql_row[3] &&car_number==sql_row[5]) {
												if (compareDatetime(year, sql_row[1]) && compareDatetime(sql_row[3], year)) {
													printf("%s's(customer) goods in %s at %s (this truck start deliver at %s)\n",sql_row[6], sql_row[5], year.c_str(),sql_row[1]);
													count++;
												}

											}
											//수신자에게
											else if(sql_row[4] && car_number == sql_row[5]) {
												if (compareDatetime(year, sql_row[1]) && compareDatetime(sql_row[4], year)) {
													printf("%s's(customer) goods in %s at %s (this truck start deliver at %s)\n", sql_row[6], sql_row[5], year.c_str(), sql_row[1]);
													count++;
												}
											}
										}
									}
								}
								else if (option2 == 2) {
									vector<string> semi_result;
									while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
									{
										if (find(car_waybill.begin(), car_waybill.end(), sql_row[0]) != car_waybill.end())
										{
											//허브 물품 시간 체크(여기서 체크가 돼도, 수신자를 찾을수 있도록 semi_result에 추가)
											if (sql_row[3] && car_number == sql_row[5]) {
												if (compareDatetime(year, sql_row[1]) && compareDatetime(sql_row[3], year)) 
												{
													semi_result.push_back(sql_row[0]);
												}
											}
											//수신자 전송 시간 체크
											else if (sql_row[4] && car_number == sql_row[5]) {
												if (compareDatetime(year, sql_row[1]) && compareDatetime(sql_row[4], year)) 
												{
													semi_result.push_back(sql_row[0]);
												}
											}
											//트럭 폭발시 운송장 번호를 가지고 있는 semi_result의에서..
											if (find(semi_result.begin(), semi_result.end(), sql_row[0]) != semi_result.end()) {
												//수신자 정보 찾을수 있는 tuple보면 출력
												if (sql_row[4]) {
													printf("%s's(receiver) goods in %s at %s (receive at %s)\n", sql_row[7], car_number.c_str(), year.c_str(), sql_row[4]);
													count++;
												}
											}
										}
									}
								}
								else if (option2 == 3) {
									//arrival_time낮은것 부터 정렬. 
									sort(arrival_time.begin(), arrival_time.end());
									//충돌시간보다 작은것중 가장 큰거. 그니깐 year보다 작으면 계속 업데이트 하다 큰거 만나면 break
									string last_day="0000-00-00 00:00:00";
									string zz="";
									for (int i = 0; i < arrival_time.size(); i++) {
										if (compareDatetime(year, arrival_time[i].first)) {
											last_day = arrival_time[i].first;
											zz = arrival_time[i].second;
										}
										else {
											break;
										}
									}
									if (zz == "") {
										count = 0;
									}
									else
									{
										count = 1;
										while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
										{
											if (sql_row[0] == zz) {
												if (sql_row[3] && sql_row[3]==last_day) {
													printf("If destroy at %s then last successful deliver of %s(truck) is %s.(Success at %s)", year.c_str(), sql_row[5], sql_row[0], sql_row[3]);
													break;
												}
												else if (sql_row[4] && sql_row[4] == last_day){
													printf("If destroy at %s then last successful deliver of %s(truck) is %s.(Success at %s)", year.c_str(), sql_row[5], sql_row[0], sql_row[4]);
													break;
												}
											}
										}
									}
								}
								if (count == 0)
									cout << "none\n";
								mysql_free_result(sql_result);
								arrival_time.clear();
								count = 0;
							}
						}
					}
				}
				else {
					cout << "error\n";
				}
			}
			else if (option == 2) {
				printf("\n\n");
				printf("---- TYPE 2 ----\n\n");
				printf("** Find the customer who has shipped the most packages in the certain year.**\n");
				printf(" Which year? : ");
				cin >> year;
				query = "SELECT c_id, COUNT(*) AS count FROM buy WHERE YEAR(payment_data) = ";
				query += year;
				query += " GROUP BY c_id ORDER BY count DESC LIMIT 1";
				int state = 0;
				state = mysql_query(connection, query.c_str());

				if (state == 0)
				{
					cout << "At " << year << " most shipped customer is: ";
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s, %ss\n", sql_row[0], sql_row[1]);
						count++;
					}
					if (count == 0) {
						printf("none\n");
					}
					mysql_free_result(sql_result);
				}
				else {
					cout << "error\n";
				}
				printf("\n");
			}
			else if (option == 3) {
				printf("\n\n");
				printf("---- TYPE 3 ----\n\n");
				printf("** Find the customer who has spent the most money in the past year.**\n");
				printf(" Which year? : ");
				cin >> year;
				query =
					"SELECT b.c_ID, SUM(sb.deliver_price + g.price*b.quantity) AS total_payment  "
					"FROM buy b "
					"JOIN goods g ON b.goods_name = g.name AND b.s_ID=g.ID "
					"JOIN service_buy sb ON b.deliver_type = sb.deliver_type AND b.receiver_address = sb.receiver_address "
					"WHERE YEAR(b.payment_data) = '" + year + "'"
					"GROUP BY b.c_ID "
					"ORDER BY total_payment DESC "
					"LIMIT 1 ";
				int state = 0;
				state = mysql_query(connection, query.c_str());
				if (state == 0)
				{
					cout << "At " << year << " most spent money customer is: ";
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						printf("%s (spent %swon)\n", sql_row[0], sql_row[1]);
						count++;
					}
					if (count == 0) {
						printf("none\n");
					}
					mysql_free_result(sql_result);
				}
				else {
					cout << "error\n";
				}
				printf("\n");
			}
			else if (option == 4) {
				printf("\n\n");
				printf("---- TYPE 4 ----\n\n");
				printf("** Find those packages that were not delivered within the promised time..**\n");
				query = "SELECT ready_deliver.c_id, ready_deliver.goods_name, ready_deliver.expected_arrival, receiver.arrival_data "
					"From ready_deliver NATURAL JOIN receiver "
					"WHERE ready_deliver.expected_arrival >= receiver.arrival_data";
				int state = 0;
				state = mysql_query(connection, query.c_str());

				if (state == 0)
				{
					cout << " result is:  \n";
					sql_result = mysql_store_result(connection);

					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{
						//if(compareD(one,two))
						printf("%s's %s arrived at %s (expected: %s)\n", sql_row[0], sql_row[1], sql_row[3], sql_row[2]);
						count++;

					}
					if (count == 0)
						cout << "none\n";
					mysql_free_result(sql_result);
				}
				else {
					cout << "error\n";
				}
				printf("\n");
			}
			else if (option == 5) {
				printf("\n\n");
				string year, month;
				printf("---- TYPE 5 ----\n\n");
				printf("** Bill at year-month..**\n");
				printf(" Input year and month(split by ' ') : ");
				cin >> year >> month;
				//buy.c_id, buy.goods_name, goods.price, service_buy.deliver_price, buy.deliver_type, buy.payment_method, buy.receiver_phone, receiver.arrival_data
				query = "SELECT buy.c_id, buy.goods_name, service_buy.deliver_price , goods.price , buy.payment_method, buy.receiver_phone, receiver.arrival_data, buy.payment_data "
					"From buy LEFT OUTER JOIN service_buy ON buy.deliver_type =service_buy.deliver_type AND buy.receiver_address=service_buy.receiver_address "
					"LEFT OUTER JOIN customer ON buy.c_id = customer.id "
					"LEFT OUTER JOIN goods ON buy.s_id =goods.id AND buy.goods_name=goods.name "
					"LEFT OUTER JOIN ready_deliver ON buy.c_id =ready_deliver.c_id AND buy.s_id =ready_deliver.s_id AND buy.goods_name =ready_deliver.goods_name AND buy.deliver_type =ready_deliver.deliver_type AND buy.payment_data =ready_deliver.payment_data "
					"LEFT OUTER JOIN receiver ON ready_deliver.waybill_number =receiver.waybill_number AND receiver.goods_name=ready_deliver.goods_name "
					"WHERE YEAR(buy.payment_data) = '" + year + "' AND MONTH(buy.payment_data) = '"+month+ "'";
				int state = 0;
				int first = 0;
				state = mysql_query(connection, query.c_str());
				if (state == 0)
				{
					sql_result = mysql_store_result(connection);
					while ((sql_row = mysql_fetch_row(sql_result)) != NULL)
					{	
						if (first == 0) {
							first = 1;
							printf(" customer_id |     goods    | goods price | deliver price | payment_method | receiver phone |     pyament date     | arrival_date\n");
							printf(" ---------------------------------------------------------------------------------------------------\n");
						}
						printf("%-14s %-14s %-14s %-14s %-17s %-14s %-24s", sql_row[0], sql_row[1], sql_row[3], sql_row[2], sql_row[4], sql_row[5], sql_row[7]);
						if (sql_row[6]) {
							printf("%s\n", sql_row[6]);
						}
						else {
							printf("not delivered\n");
						}
						count++;
						
					}
					if (count == 0) {
						printf("none\n");
					}
					mysql_free_result(sql_result);
				}
				else {
					cout << "error\n";
				}
				printf("\n");
			}
			else {
				printf("wrong option!\n\n");
			}
		}

		mysql_close(connection);

	}
	//마지막으로 있던것 drop
	mysql_init(&conn);

	if (!mysql_real_connect(&conn, host, user, pw, db, 0, NULL, 0))
	{
		cerr << "MySQL 연결 실패: " << mysql_error(&conn) << endl;
		return 1;
	}

	filename = "drops.sql";
	ifstream filess(filename);

	if (!filess.is_open()) {
		cerr << "파일 열기 실패" << endl;
		return 1;
	}

	//string line;
	while (getline(filess, line)) {
		if (line == "")
			continue;
		if (mysql_query(&conn, line.c_str()) != 0) {
			cerr << count << " : 쿼리 실행 실패: " << mysql_error(&conn) << endl;
			return 1;
		}
	}
	cout << "drop success! \nbye bye!\n";
	filess.close();
	mysql_close(&conn);

	return 0;
}





/*

	printf("------- SELECT QUERY TYPES -------\n\n");
	printf("\t1. TYPE 1\n");
	printf("\t2. TYPE 2\n");
	printf("\t3. TYPE 3\n");
	printf("\t4. TYPE 4\n");
	printf("\t5. TYPE 5\n");
	printf("\t6. TYPE 6\n");
	printf("\t7. TYPE 7\n");
	printf("\t0. QUIT\n");
	//printf("----------------------------------\n");
	printf("\n\n");
	printf("---- TYPE 5 ----\n\n");
	printf("** Find the top k brands by unit sales by the year**\n");
	printf(" Which K? : 3\n");

	return 0;

}
*/
