CREATE TABLE buy
(
	c_ID                 VARCHAR(20) NOT NULL,
	payment_data         DATETIME NOT NULL,
	goods_name           VARCHAR(30) NOT NULL,
	s_ID                 VARCHAR(30) NOT NULL,
	quantity             INTEGER NOT NULL,
	payment_method       CHAR(30) NOT NULL,
	deliver_type         INTEGER NOT NULL,
	receiver_address     VARCHAR(30) NOT NULL,
	receiver_phone       CHAR(30) NULL
);
ALTER TABLE buy ADD PRIMARY KEY (c_ID, s_ID, goods_name, deliver_type, receiver_address, payment_data);

CREATE TABLE customer
(
	ID                   VARCHAR(20) NOT NULL,
	address              VARCHAR(30) NOT NULL,
	name                 VARCHAR(20) NOT NULL,
	phone_number         VARCHAR(30) NULL,
	customs_code         VARCHAR(30) NULL
);

ALTER TABLE customer ADD PRIMARY KEY (ID);

CREATE TABLE deliver_assgn
(
	waybill_number       INTEGER NOT NULL,
	goods_name           VARCHAR(30) NOT NULL,
	depart_time          DATETIME NULL,
	d_ID                 VARCHAR(30) NOT NULL,
	destination          VARCHAR(18) NOT NULL
);

ALTER TABLE deliver_assgn ADD PRIMARY KEY (waybill_number,d_ID,goods_name,destination);

CREATE TABLE deliver_hub
(
	waybill_number       INTEGER NOT NULL,
	goods_name           VARCHAR(30) NOT NULL,
	d_ID                 VARCHAR(30) NOT NULL,
	start_hub            VARCHAR(18) NOT NULL
);

ALTER TABLE deliver_hub ADD PRIMARY KEY (waybill_number,goods_name,start_hub,d_ID);

CREATE TABLE driver
(
	ID                   VARCHAR(30) NOT NULL,
	driver_number        VARCHAR(30) NOT NULL,
	car_number           VARCHAR(20) NOT NULL,
	name                 VARCHAR(30) NOT NULL,
	career               INTEGER NOT NULL,
	addresss             VARCHAR(30) NOT NULL
);

ALTER TABLE driver ADD PRIMARY KEY (ID);

CREATE TABLE goods
(
	name                 VARCHAR(30) NOT NULL,
	ID                   VARCHAR(30) NOT NULL,
	register_data        DATE NOT NULL,
	price                INTEGER NOT NULL
);

ALTER TABLE goods ADD PRIMARY KEY (name,ID);

CREATE TABLE hub
(
	waybill_number       INTEGER NOT NULL,
	goods_name           VARCHAR(30) NOT NULL,
	ceo                  VARCHAR(30) NOT NULL,
	arrival_time         DATETIME NOT NULL,
	d_ID                 VARCHAR(30) NOT NULL,
	address              VARCHAR(18) NOT NULL
);

ALTER TABLE hub ADD PRIMARY KEY (waybill_number,goods_name,address);

CREATE TABLE info_goods
(
	name                 VARCHAR(30) NOT NULL,
	weight               FLOAT NOT NULL,
	danger_class         INTEGER NOT NULL
);

ALTER TABLE info_goods ADD PRIMARY KEY (name);

CREATE TABLE ready_deliver
(
	waybill_number       INTEGER NOT NULL,
	expected_arrival     DATETIME NOT NULL,
	c_ID                 VARCHAR(20) NOT NULL,
	s_ID                 VARCHAR(30) NULL,
	goods_name           VARCHAR(30) NOT NULL,
	deliver_type         INTEGER NULL,
	receiver_address     VARCHAR(30) NULL,
	payment_data         DATETIME NULL
);

ALTER TABLE ready_deliver ADD PRIMARY KEY (waybill_number,goods_name);

CREATE TABLE receiver
(
	arrival_data         DATETIME NULL,
	name                 VARCHAR(30) NOT NULL,
	waybill_number       INTEGER NOT NULL,
	d_ID                 VARCHAR(30) NOT NULL,
	goods_name           VARCHAR(30) NOT NULL,
	address              VARCHAR(18) NOT NULL
);

ALTER TABLE receiver ADD PRIMARY KEY (waybill_number,goods_name,address);

CREATE TABLE seller
(
	ID                   VARCHAR(30) NOT NULL,
	name                 VARCHAR(10) NOT NULL,
	address              VARCHAR(20) NOT NULL,
	company              VARCHAR(20) NULL
);

ALTER TABLE seller ADD PRIMARY KEY (ID);

CREATE TABLE service_buy
(
	deliver_type         INTEGER NOT NULL,
	receiver_address     VARCHAR(30) NOT NULL,
	deliver_price        INTEGER NOT NULL,
	packing_method       VARCHAR(30) NOT NULL
);

ALTER TABLE service_buy ADD PRIMARY KEY (deliver_type,receiver_address);

ALTER TABLE buy ADD CONSTRAINT R_52 FOREIGN KEY (c_ID) REFERENCES customer (ID);

ALTER TABLE buy ADD CONSTRAINT R_57 FOREIGN KEY (goods_name, s_ID) REFERENCES goods (name, ID);

ALTER TABLE buy ADD CONSTRAINT R_59 FOREIGN KEY (deliver_type, receiver_address) REFERENCES service_buy (deliver_type, receiver_address);

ALTER TABLE deliver_assgn ADD CONSTRAINT R_62 FOREIGN KEY (waybill_number, goods_name) REFERENCES ready_deliver (waybill_number, goods_name);

ALTER TABLE deliver_assgn ADD CONSTRAINT R_64 FOREIGN KEY (d_ID) REFERENCES driver (ID);

ALTER TABLE deliver_hub ADD CONSTRAINT R_70 FOREIGN KEY (waybill_number, goods_name, start_hub) REFERENCES hub (waybill_number, goods_name, address);

ALTER TABLE deliver_hub ADD CONSTRAINT R_74 FOREIGN KEY (d_ID) REFERENCES driver (ID);

ALTER TABLE deliver_hub ADD CONSTRAINT R_75 FOREIGN KEY (waybill_number, d_ID, goods_name) REFERENCES deliver_assgn (waybill_number, d_ID, goods_name);

ALTER TABLE goods ADD CONSTRAINT R_46 FOREIGN KEY (ID) REFERENCES seller (ID);

ALTER TABLE goods ADD CONSTRAINT R_50 FOREIGN KEY (name) REFERENCES info_goods (name);

ALTER TABLE hub ADD CONSTRAINT R_22 FOREIGN KEY (waybill_number, d_ID, goods_name, address) REFERENCES deliver_assgn (waybill_number, d_ID, goods_name, destination);

ALTER TABLE ready_deliver ADD CONSTRAINT R_61 FOREIGN KEY (c_ID, s_ID, goods_name, deliver_type, receiver_address, payment_data) REFERENCES buy (c_ID, s_ID, goods_name, deliver_type, receiver_address, payment_data);

ALTER TABLE receiver ADD CONSTRAINT R_23 FOREIGN KEY (waybill_number, d_ID, goods_name, address) REFERENCES deliver_assgn (waybill_number, d_ID, goods_name, destination);
