모든 파일을 제출하면 용량이 너무 커지기에, 환경설정은 조교님이 강의에 올려주신대로 똑같은 환경에서 했으니. 코드와 input 파일들만 제출하겟습니다.

0으로 정상 종료가 아닌 강제종료가 될시 drop.sql을 읽지 않아서 drop 되지 않습니다.

오류 예시 : ^C로 종료, 닫기 창으로 종료, datetime input오류시 drop 못함.

create.sql : table create 쿼리들입니다. erwin에서 생성된것 그대로 사용
insert.sql : table에 tuple insert하는 쿼리입니다. 각 테이블당 평균 10개씩 삽입
drop.sql : 모든 table drop하는 쿼리입니다. 