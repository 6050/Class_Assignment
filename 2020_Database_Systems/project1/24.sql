SELECT C.name AS '각 시티의 이름', AVG(level) AS '트레이너들이 잡은 포켓몬들의 평균 레벨'
FROM City AS C, Trainer AS T, CatchedPokemon
WHERE C.name = hometown AND T.id = owner_id
GROUP BY C.name
ORDER BY AVG(level);