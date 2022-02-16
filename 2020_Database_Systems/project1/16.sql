SELECT COUNT(*) AS '물 타입, 전기 타입, 에스퍼 타입 포켓몬 수의 총합'
FROM Pokemon
WHERE type = 'Water' OR type = 'Electric' OR type = 'Psychic';