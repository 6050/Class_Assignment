SELECT name AS '풀 타입 포켓몬 중 진화 가능한 포켓몬 이름'
FROM Pokemon, Evolution
WHERE id = before_id AND type = 'Grass';