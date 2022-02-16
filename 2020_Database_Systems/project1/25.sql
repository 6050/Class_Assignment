SELECT DISTINCT P.name AS '상록시티, 브라운 시티 출신 트레이너가 공통으로 잡은 포켓몬의 이름'
FROM Trainer AS T, CatchedPokemon, Pokemon AS P
WHERE hometown = 'Sangnok City' AND T.id = owner_id AND pid = P.id AND P.id IN (
  SELECT P.id
  FROM Trainer AS T, CatchedPokemon , Pokemon AS P
  WHERE hometown = 'Brown City' AND T.id = owner_id AND pid = P.id
  )
ORDER BY P.name;