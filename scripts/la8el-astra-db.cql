USE la8el ;

DROP TABLE IF EXISTS anchors ;

CREATE TABLE IF NOT EXISTS anchors (long_id text, 
                                    mac_address text, 
                                    short_id text, 
                                    location text, 
                                    latitude float, 
                                    longitude float, 
                                    altitude float, 
                                    position_x int, 
                                    position_y int, 
                                    position_z int, 
                                    azimuth smallint,
                                    calibration float, 
                                    timestamp text, 
                                    note text, 
                                    status tinyint, 
                                    PRIMARY KEY (long_id, mac_address)) ;

DROP TABLE IF EXISTS anchors_log ;

CREATE TABLE IF NOT EXISTS anchors_log (id uuid,
                                       long_id text, 
                                       mac_address text, 
                                       year int, 
                                       month int, 
                                       date int, 
                                       hour int, 
                                       minute int, 
                                       second int, 
                                       short_id text, 
                                       location text, 
                                       latitude float, 
                                       longitude float, 
                                       altitude float, 
                                       position_x int, 
                                       position_y int, 
                                       position_z int, 
                                       azimuth smallint,
                                       calibration float, 
                                       timestamp text, 
                                       note text, 
                                       status tinyint, 
                                       PRIMARY KEY ((id, date, hour), minute, second)) 
                                       WITH CLUSTERING ORDER BY (minute DESC, second DESC);

DROP TABLE IF EXISTS tags ;

CREATE TABLE IF NOT EXISTS tags (long_id text, 
                                 mac_address text, 
                                 short_id text, 
                                 location text, 
                                 latitude float, 
                                 longitude float, 
                                 altitude float, 
                                 position_x int, 
                                 position_y int, 
                                 position_z int, 
                                 azimuth smallint,
                                 calibration float, 
                                 timestamp text, 
                                 note text, 
                                 status tinyint, 
                                 PRIMARY KEY (long_id, mac_address)) ;

DROP TABLE IF EXISTS tags_log ;

CREATE TABLE IF NOT EXISTS tags_log (id uuid,
                                     long_id text, 
                                     mac_address text, 
                                     year int, 
                                     month int, 
                                     date int, 
                                     hour int, 
                                     minute int, 
                                     second int, 
                                     short_id text, 
                                     location text, 
                                     latitude float, 
                                     longitude float, 
                                     altitude float, 
                                     position_x int, 
                                     position_y int, 
                                     position_z int, 
                                     azimuth smallint,
                                     calibration float, 
                                     timestamp text, 
                                     note text, 
                                     status tinyint, 
                                     PRIMARY KEY ((id, date, hour), minute, second)) 
                                     WITH CLUSTERING ORDER BY (minute DESC, second DESC);
