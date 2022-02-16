SELECT SUM(level) AS '트레이너 Matis가 잡은 포켓몬들의 레벨의 총합'
FROM Trainer AS T, CatchedPokemon
WHERE name = 'Matis' AND T.id = owner_id;