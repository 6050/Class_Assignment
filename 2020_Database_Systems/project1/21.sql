SELECT name AS '각 관장의 이름', COUNT(*) AS '잡은 포켓몬 수'
FROM Trainer AS T, Gym, CatchedPokemon
WHERE T.id = leader_id AND T.id = owner_id
GROUP BY name
ORDER BY name;