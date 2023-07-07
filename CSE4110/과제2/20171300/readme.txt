0으로 정상 종료가 아닌 강제종료가 될시 drop.sql을 읽지 않아서 drop 되지 않습니다.

오류 예시 : ^C로 종료, 닫기 창으로 종료, datetime input오류시 drop 못함.

create.sql : table create 쿼리들입니다. erwin에서 생성된것 그대로 사용
insert.sql : table에 tuple insert하는 쿼리입니다. 각 테이블당 평균 10개씩 삽입
drop.sql : 모든 table drop하는 쿼리입니다. 
