SELECT name AS '동일한 포켓몬을 두 마리 이상 잡은 트레이너의 이름'
FROM Trainer AS T, CatchedPokemon
WHERE T.id = owner_id
GROUP BY name
HAVING COUNT(pid) <> COUNT(DISTINCT pid)
ORDER BY name;