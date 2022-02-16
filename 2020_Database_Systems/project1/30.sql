SELECT P1.id AS '3단 진화가 가능한 포켓몬의 ID', P1.name AS '1단 진화 형태 포켓몬의 이름', P2.name AS '2단 진화 형태 포켓몬의 이름', P3.name AS '3단 진화 형태 포켓몬의 이름'
FROM Pokemon AS P1, Pokemon AS P2, Pokemon AS P3, Evolution AS E1, Evolution AS E2
WHERE P1.id = E1.before_id AND P2.id = E1.after_id AND P3.id = E2.after_id AND E2.before_id = E1.after_id
ORDER BY P1.id;