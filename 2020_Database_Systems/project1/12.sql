SELECT DISTINCT name AS '레벨이 30 이상인 포켓몬의 이름', type AS '타입'
FROM Pokemon AS P, CatchedPokemon
WHERE P.id = pid AND level >= 30
ORDER BY name;