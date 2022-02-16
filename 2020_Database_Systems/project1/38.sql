SELECT name AS '최종 진화 형태의 포켓몬의 이름'
FROM Pokemon, Evolution
WHERE id = after_id AND after_id NOT IN (SELECT before_id FROM Evolution)
ORDER BY name;