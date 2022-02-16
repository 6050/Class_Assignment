SELECT name AS '진화 후의 ID가 더 작아지는 포켓몬의 진화 전 이름'
FROM Evolution AS E, Pokemon
WHERE id = E.before_id AND E.before_id > E.after_id
ORDER BY name;